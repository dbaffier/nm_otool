ofile_map(
		const char *file_name,
		const struct arch_flag *arch_flag,	/* can be NULL */
		const char *object_name,		/* can be NULL */
		struct ofile *ofile,
		enum bool archives_with_fat_objects)
{
	int fd;
	struct stat stat_buf;
	unsigned long i, size, magic;
	kern_return_t r;
	char *addr;
	enum byte_sex host_byte_sex;
	struct arch_flag host_arch_flag;
	enum bool family;
	const struct arch_flag *family_arch_flag;

	magic = 0; /* to shut up the compiler warning message */
	memset(ofile, '\0', sizeof(struct ofile));

	/* Open the file and map it in */
	/* fill in the start of the ofile structure */
	ofile->file_name = savestr(file_name);
	if(ofile->file_name == NULL)
		return(FALSE);
	ofile->file_addr = addr;
	ofile->file_size = size;

	/* Try to figure out what kind of file this is */

	if(size >= sizeof(unsigned long)){
		magic = *((unsigned long *)addr);
	}
	host_byte_sex = get_host_byte_sex();

	/* see if this file is a fat file (always in big endian byte sex) */
#ifdef __BIG_ENDIAN__
	if(size >= sizeof(struct fat_header) && magic == FAT_MAGIC)
#endif /* __BIG_ENDIAN__ */
#ifdef __LITTLE_ENDIAN__
		if(size >= sizeof(struct fat_header) && SWAP_LONG(magic) == FAT_MAGIC)
#endif /* __LITTLE_ENDIAN__ */
		{
			ofile->file_type = OFILE_FAT;
			ofile->fat_header = (struct fat_header *)addr;
#ifdef __LITTLE_ENDIAN__
			swap_fat_header(ofile->fat_header, host_byte_sex);
#endif /* __LITTLE_ENDIAN__ */
#ifdef OTOOL
			if(otool_first_ofile_map && fflag)
				printf("Fat headers\n");
#endif /* OTOOL */
			ofile->fat_archs = (struct fat_arch *)(addr +
					sizeof(struct fat_header));
#ifdef __LITTLE_ENDIAN__
			swap_fat_arch(ofile->fat_archs, ofile->fat_header->nfat_arch,
					host_byte_sex);
#endif /* __LITTLE_ENDIAN__ */
#ifdef OTOOL
			if(otool_first_ofile_map && fflag)
				print_fat_headers(ofile->fat_header, ofile->fat_archs,
						size, vflag);
#endif /* OTOOL */
			if(check_fat(ofile) == CHECK_BAD){
				ofile_unmap(ofile);
				return(FALSE);
#endif
			}
			/*
			 * Now that the fat file is mapped fill in the ofile to the level
			 * the caller wants based on the arch_flag and object_name passed.
			 * If the caller did not specify an arch_flag or an object_name
			 * then everything the caller wants is done.
			 */
			ofile->arch_flag.name = savestr(arch_flag->name);
			if(ofile->arch_flag.name == NULL)
				goto cleanup;
			ofile->arch_flag.cputype = arch_flag->cputype;
			ofile->arch_flag.cpusubtype = arch_flag->cpusubtype;

			ofile->narch = ULONG_MAX;
			if(ofile->narch == ULONG_MAX){
				family = FALSE;
				family_arch_flag =
					get_arch_family_from_cputype(ofile->arch_flag.cputype);
				if(family_arch_flag != NULL)
					family = (enum bool)(family_arch_flag->cpusubtype ==
							ofile->arch_flag.cpusubtype);
				ofile->narch = ULONG_MAX;
				for(i = 0; i < ofile->fat_header->nfat_arch; i++){
					if(ofile->fat_archs[i].cputype ==
							ofile->arch_flag.cputype &&
							(family == TRUE || ofile->fat_archs[i].cpusubtype ==
							 ofile->arch_flag.cpusubtype)){
						ofile->arch_flag.cpusubtype =
							ofile->fat_archs[i].cpusubtype;
						ofile->narch = i;
						break;
					}
				}
			}
			if(ofile->narch == ULONG_MAX){
				error("fat file: %s does not contain architecture %s",
						ofile->file_name, arch_flag->name);
				ofile_unmap(ofile);
				return(FALSE);
			}
			/* Now determine the file type for this specific architecture */
			size = ofile->fat_archs[i].size;
			addr = addr + ofile->fat_archs[i].offset;
			if(size >= sizeof(struct mach_header))
				memcpy(&magic, addr, sizeof(unsigned long));
			/* see if this file is Mach-O file */
			if(size >= sizeof(struct mach_header) &&
					(magic == MH_MAGIC || magic == SWAP_LONG(MH_MAGIC))){
#ifdef ALIGNMENT_CHECKS
				if(ofile->fat_archs[i].offset % sizeof(unsigned long) != 0){
					error("fat file: %s architecture %s malformed for an "
							"object file (offset is not a multiple of sizeof("
							"unsigned long))", ofile->file_name, arch_flag->name);
					ofile_unmap(ofile);
#ifdef OFI
					return(NSObjectFileImageFormat);
#else
					return(FALSE);
#endif
				}
#endif /* ALIGNMENT_CHECKS */
				ofile->arch_type = OFILE_Mach_O;
				ofile->object_addr = addr;
				ofile->object_size = size;
				if(magic == MH_MAGIC)
					ofile->object_byte_sex = host_byte_sex;
				else
					ofile->object_byte_sex =
						host_byte_sex == BIG_ENDIAN_BYTE_SEX ?
						LITTLE_ENDIAN_BYTE_SEX : BIG_ENDIAN_BYTE_SEX;
				ofile->mh = (struct mach_header *)addr;
				ofile->load_commands = (struct load_command *)(addr +
						sizeof(struct mach_header));
				if(check_Mach_O(ofile) == CHECK_BAD){
					ofile_unmap(ofile);
#ifdef OFI
					return(NSObjectFileImageFormat);
#else
					return(FALSE);
#endif
				}
				if(object_name != NULL){
					error("fat file: %s architecture %s is not an archive "
							"(object_name to ofile_map() can't be specified to "
							"be other than NULL)", ofile->file_name,
							arch_flag->name);
					goto cleanup;
				}
			}
			/* see if this file is an archive file */
			else if(size >= SARMAG && strncmp(addr, ARMAG, SARMAG) == 0){
				ofile->arch_type = OFILE_ARCHIVE;
				if(check_archive(ofile, FALSE) == CHECK_BAD){
					ofile_unmap(ofile);
#ifdef OFI
					return(NSObjectFileImageInappropriateFile);
#else
					return(FALSE);
#endif
				}
#ifdef ALIGNMENT_CHECKS
				if(ofile->archive_cputype != 0 &&
						ofile->fat_archs[i].offset % sizeof(unsigned long) != 0){
					error("fat file: %s architecture %s malformed archive that "
							"contains object files (offset to archive is not a "
							"multiple of sizeof(unsigned long))",
							ofile->file_name, arch_flag->name);
					ofile_unmap(ofile);
#ifdef OFI
					return(NSObjectFileImageInappropriateFile);
#else
					return(FALSE);
#endif
				}
#endif /* ALIGNMENT_CHECKS */
				if(object_name != NULL){
					if(ofile_specific_member(object_name, ofile) == FALSE)
						goto cleanup;
				}
			}
			/* this file type is now known to be unknown to this program */
			else{
				ofile->file_type = OFILE_UNKNOWN;
				if(object_name != NULL){
					error("fat file: %s architecture %s is not an archive "
							"(object_name to ofile_map() can't be specified to "
							"be other than NULL)", ofile->file_name,
							arch_flag->name);
					goto cleanup;
				}
			}
		}
	/* see if this file is Mach-O file */
		else if(size >= sizeof(struct mach_header) &&
				(magic == MH_MAGIC || magic == SWAP_LONG(MH_MAGIC))){
			ofile->file_type = OFILE_Mach_O;
			ofile->object_addr = addr;
			ofile->object_size = size;
			if(magic == MH_MAGIC)
				ofile->object_byte_sex = host_byte_sex;
			else
				ofile->object_byte_sex = host_byte_sex == BIG_ENDIAN_BYTE_SEX ?
					LITTLE_ENDIAN_BYTE_SEX : BIG_ENDIAN_BYTE_SEX;
			ofile->mh = (struct mach_header *)addr;
			ofile->load_commands = (struct load_command *)(addr +
					sizeof(struct mach_header));
			if(check_Mach_O(ofile) == CHECK_BAD){
				ofile_unmap(ofile);
#ifdef OFI
				return(NSObjectFileImageFormat);
#else
				return(FALSE);
#endif
			}
		}
	/* see if this file is an archive file */
		else if(size >= SARMAG && strncmp(addr, ARMAG, SARMAG) == 0){
			ofile->file_type = OFILE_ARCHIVE;
			if(check_archive(ofile, archives_with_fat_objects) == CHECK_BAD)
				goto cleanup;
			if(object_name != NULL){
				if(ofile_specific_member(object_name, ofile) == FALSE)
					goto cleanup;
				if(arch_flag != NULL){
					if(arch_flag->cputype != ofile->mh->cputype &&
							arch_flag->cpusubtype != ofile->mh->cpusubtype){
						error("object file: %s(%.*s) does not match specified "
								"arch_flag: %s passed to ofile_map()",
								ofile->file_name, (int)ofile->member_name_size,
								ofile->member_name, arch_flag->name);
						goto cleanup;
					}
				}
			}
			else{
				if(arch_flag != NULL){
					if(arch_flag->cputype != ofile->archive_cputype &&
							arch_flag->cpusubtype != ofile->archive_cpusubtype){
						error("archive file: %s objects do not match specified "
								"arch_flag: %s passed to ofile_map()",
								ofile->file_name, arch_flag->name);
						goto cleanup;
					}
				}
			}
		}
	/* this file type is now known to be unknown to this program */
		else{
#ifndef OTOOL
unknown:
#endif
			ofile->file_type = OFILE_UNKNOWN;
			if(arch_flag != NULL){
#ifdef OFI
				ofile_unmap(ofile);
				return(NSObjectFileImageInappropriateFile);
#else
				error("file: %s is unknown type (arch_flag to ofile_map() "
						"can't be specified to be other than NULL)",
						ofile->file_name);
				ofile_unmap(ofile);
				return(FALSE);
#endif
			}
			if(object_name != NULL){
				error("file: %s is not an archive (object_name to ofile_map() "
						"can't be specified to be other than NULL)",
						ofile->file_name);
				goto cleanup;
			}
		}
success:
	return(TRUE);

cleanup:
	ofile_unmap(ofile);
	return(FALSE);
}




if(ofile.file_type == OFILE_FAT){
	/*
	 * This is a fat file so see if a list of architecture is
	 * specified and process only those.
	 */
	if(all_archs == FALSE && narch_flags != 0){

		family = FALSE;
		if(narch_flags == 1){
			family_arch_flag =
				get_arch_family_from_cputype(arch_flags[0].cputype);
			if(family_arch_flag != NULL)
				family = (enum bool)(family_arch_flag->cpusubtype ==
						arch_flags[0].cpusubtype);
		}

		for(i = 0; i < narch_flags; i++){
			(void)ofile_first_arch(&ofile);
			arch_found = FALSE;
			if(narch_flags != 1)
				arch_name = ofile.arch_flag.name;
			else
				arch_name = NULL;
			do{
				if(ofile.arch_flag.cputype ==
						arch_flags[i].cputype &&
						(ofile.arch_flag.cpusubtype ==
						 arch_flags[i].cpusubtype || family == TRUE)){
					arch_found = TRUE;
					if(ofile.arch_type == OFILE_ARCHIVE){
						if(member_name != NULL){
							if(ofile_specific_member(member_name,
										&ofile) == TRUE)
								processor(&ofile, arch_name, cookie);
						}
						else{
							/* loop through archive */
#ifdef OTOOL
							printf("Archive : %s", ofile.file_name);
							if(arch_name != NULL)
								printf(" (architecture %s)",
										arch_name);
							printf("\n");
#endif /* OTOOL */
							if(ofile_first_member(&ofile) == TRUE){
								flag = FALSE;
								do{
									if(process_non_objects == TRUE ||
											ofile.member_type ==
											OFILE_Mach_O){
										processor(&ofile, arch_name,
												cookie);
										flag = TRUE;
									}
								}while(ofile_next_member(&ofile) ==
										TRUE);
								if(flag == FALSE){
									error("for architecture: %s "
											"archive: %s contains no "
											"members that are object "
											"files", ofile.arch_flag.name,
											ofile.file_name);
								}
							}
							else{
								error("for architecture: %s archive: "
										"%s contains no members",
										ofile.arch_flag.name,
										ofile.file_name);
							}
						}
					}
					else if(process_non_objects == TRUE ||
							ofile.arch_type == OFILE_Mach_O){
						if(ofile.arch_type == OFILE_Mach_O &&
								ofile.mh->filetype == MH_DYLIB){
							if(dylib_flat == TRUE){
								processor(&ofile, arch_name, cookie);
							}
							else{
								if(member_name != NULL){
									if(ofile_specific_module(
												member_name, &ofile) == TRUE)
										processor(&ofile, arch_name,
												cookie);
								}
								else{
									/*loop through the dynamic library*/
									if(ofile_first_module(&ofile)){
										do{
											processor(&ofile, arch_name,
													cookie);
										}while(ofile_next_module(
													&ofile));
									}
									else{
										error("for architecture: %s "
												"dynamic library: %s "
												"contains no modules",
												ofile.arch_flag.name,
												ofile.file_name);
									}
								}
							}
						}
						else{
							if(member_name != NULL)
								error("for architecture: %s file: %s "
										"is not an archive and thus does "
										"not contain member: %s",
										ofile.arch_flag.name,
										ofile.file_name,
										member_name);
							else
								processor(&ofile, arch_name, cookie);
						}
					}
					else if(ofile.arch_type == OFILE_UNKNOWN){
						error("for architecture: %s file: %s is "
								"not an object file",
								ofile.arch_flag.name,ofile.file_name);
					}
					break;
				}
			}while(ofile_next_arch(&ofile) == TRUE);
			if(arch_found == FALSE)
				error("file: %s does not contain architecture: %s",
						ofile.file_name, arch_flags[i].name);
		}
		return;
	}

	/*
	 * This is a fat file and no architectures has been specified
	 * so if it contains the host architecture process only that
	 * architecture but if not process all architectures
	 * specified.
	 */
	if(all_archs == FALSE){
		(void)get_arch_from_host(&host_arch_flag, NULL);
		hostflag = FALSE;

		family = FALSE;
		family_arch_flag =
			get_arch_family_from_cputype(host_arch_flag.cputype);
		if(family_arch_flag != NULL)
			family = (enum bool)(family_arch_flag->cpusubtype ==
					host_arch_flag.cpusubtype);

		ofile_unmap(&ofile);
		if(ofile_map(name, NULL, NULL, &ofile, FALSE) == FALSE)
			return;
		(void)ofile_first_arch(&ofile);
		do{
			if(ofile.arch_flag.cputype ==
					host_arch_flag.cputype &&
					(ofile.arch_flag.cpusubtype ==
					 host_arch_flag.cpusubtype || family == TRUE)){
				hostflag = TRUE;
				if(ofile.arch_type == OFILE_ARCHIVE){
					if(member_name != NULL){
						if(ofile_specific_member(member_name,
									&ofile) == TRUE)
							processor(&ofile, NULL, cookie);
					}
					else{
						/* loop through archive */
#ifdef OTOOL
						printf("Archive : %s\n", ofile.file_name);
#endif /* OTOOL */
						if(ofile_first_member(&ofile) == TRUE){
							flag = FALSE;
							do{
								if(process_non_objects == TRUE ||
										ofile.member_type == OFILE_Mach_O){
									processor(&ofile, NULL, cookie);
									flag = TRUE;
								}
							}while(ofile_next_member(&ofile) ==
									TRUE);
							if(flag == FALSE){
								error("archive: %s contains no "
										"members that are object "
										"files", ofile.file_name);
							}
						}
						else{
							error("archive: %s contains no "
									"members", ofile.file_name);
						}
					}
				}
				else if(process_non_objects == TRUE ||
						ofile.arch_type == OFILE_Mach_O){
					if(ofile.arch_type == OFILE_Mach_O &&
							ofile.mh->filetype == MH_DYLIB){
						if(dylib_flat == TRUE){
							processor(&ofile, NULL, cookie);
						}
						else{
							if(member_name != NULL){
								if(ofile_specific_module(member_name,
											&ofile) == TRUE)
									processor(&ofile, NULL, cookie);
							}
							else{
								/* loop through the dynamic library */
								if(ofile_first_module(&ofile) == TRUE){
									do{
										processor(&ofile, NULL, cookie);
									}while(ofile_next_module(&ofile));
								}
								else{
									error("for architecture: %s dynamic"
											" library: %s contains no "
											"modules",
											ofile.arch_flag.name,
											ofile.file_name);
								}
							}
						}
					}
					else{
						if(member_name != NULL)
							error("for architecture: %s file: %s is "
									"not an archive and thus does not "
									"contain member: %s",
									ofile.arch_flag.name, ofile.file_name,
									member_name);
						else
							processor(&ofile, NULL, cookie);
					}
				}
				else if(ofile.arch_type == OFILE_UNKNOWN){
					error("file: %s is not an object file",
							ofile.file_name);
				}
			}
		}while(hostflag == FALSE && ofile_next_arch(&ofile) == TRUE);
		if(hostflag == TRUE)
			return;
	}

	/*
	 * Either all architectures have been specified or none have
	 * been specified and it does not contain the host architecture
	 * so do all the architectures in the fat file
	 */
	ofile_unmap(&ofile);
	if(ofile_map(name, NULL, NULL, &ofile, FALSE) == FALSE)
		return;
	(void)ofile_first_arch(&ofile);
	do{
		if(ofile.arch_type == OFILE_ARCHIVE){
			if(member_name != NULL){
				if(ofile_specific_member(member_name, &ofile) == TRUE)
					processor(&ofile, ofile.arch_flag.name, cookie);
			}
			else{
				/* loop through archive */
#ifdef OTOOL
				printf("Archive : %s (architecture %s)\n",
						ofile.file_name, ofile.arch_flag.name);
#endif /* OTOOL */
				if(ofile_first_member(&ofile) == TRUE){
					flag = FALSE;
					do{
						if(process_non_objects == TRUE ||
								ofile.member_type == OFILE_Mach_O){
							processor(&ofile, ofile.arch_flag.name,
									cookie);
							flag = TRUE;
						}
					}while(ofile_next_member(&ofile) == TRUE);
					if(flag == FALSE){
						error("for architecture: %s archive: %s "
								"contains no members that are object "
								"files", ofile.arch_flag.name,
								ofile.file_name);
					}
				}
				else{
					error("for architecture: %s archive: %s "
							"contains no members",
							ofile.arch_flag.name, ofile.file_name);
				}
			}
		}
		else if(process_non_objects == TRUE ||
				ofile.arch_type == OFILE_Mach_O){
			if(ofile.arch_type == OFILE_Mach_O &&
					ofile.mh->filetype == MH_DYLIB){
				if(dylib_flat == TRUE){
					processor(&ofile, ofile.arch_flag.name, cookie);
				}
				else{
					if(member_name != NULL){
						if(ofile_specific_module(member_name, &ofile)
								== TRUE)
							processor(&ofile, ofile.arch_flag.name,
									cookie);
					}
					else{
						/* loop through the dynamic library */
						if(ofile_first_module(&ofile) == TRUE){
							do{
								processor(&ofile, ofile.arch_flag.name,
										cookie);
							}while(ofile_next_module(&ofile) == TRUE);
						}
						else{
							error("for architecture: %s dynamic library"
									": %s contains no modules",
									ofile.arch_flag.name,ofile.file_name);
						}
					}
				}
			}
			else{
				if(member_name != NULL)
					error("for architecture: %s file: %s is not an "
							"archive and thus does not contain member: "
							"%s", ofile.arch_flag.name, ofile.file_name,
							member_name);
				else
					processor(&ofile, ofile.arch_flag.name, cookie);
			}
		}
		else if(ofile.arch_type == OFILE_UNKNOWN){
			error("for architecture: %s file: %s is not an "
					"object file", ofile.arch_flag.name,
					ofile.file_name);
		}
	}while(ofile_next_arch(&ofile) == TRUE);
}
else if(ofile.file_type == OFILE_ARCHIVE){
	if(narch_flags != 0){
		arch_found = FALSE;
		for(i = 0; i < narch_flags; i++){
			family = FALSE;
			if(narch_flags == 1){
				family_arch_flag =
					get_arch_family_from_cputype(arch_flags[0].cputype);
				if(family_arch_flag != NULL)
					family = (enum bool)(family_arch_flag->cpusubtype ==
							arch_flags[0].cpusubtype);
			}
			if(ofile.archive_cputype == arch_flags[i].cputype &&
					(ofile.archive_cpusubtype == arch_flags[i].cpusubtype ||
					 family == TRUE)){
				arch_found = TRUE;
			}
			else{
				error("file: %s does not contain architecture: %s",
						ofile.file_name, arch_flags[i].name);
			}
		}
		if(arch_found == FALSE)
			return;
	}
	if(member_name != NULL){
		if(ofile_specific_member(member_name, &ofile) == TRUE)
			processor(&ofile, NULL, cookie);
	}
	else{
		/* loop through archive */
#ifdef OTOOL
		printf("Archive : %s\n", ofile.file_name);
#endif /* OTOOL */
		if(ofile_first_member(&ofile) == TRUE){
			flag = FALSE;
			do{
				if(process_non_objects == TRUE ||
						ofile.member_type == OFILE_Mach_O){
					processor(&ofile, NULL, cookie);
					flag = TRUE;
				}
			}while(ofile_next_member(&ofile) == TRUE);
			if(flag == FALSE){
				error("archive: %s contains no members that are "
						"object files", ofile.file_name);
			}
		}
		else{
			error("archive: %s contains no members",
					ofile.file_name);
		}
	}
}
else if(ofile.file_type == OFILE_Mach_O){
	if(narch_flags != 0){
		arch_found = FALSE;
		for(i = 0; i < narch_flags; i++){
			family = FALSE;
			if(narch_flags == 1){
				family_arch_flag =
					get_arch_family_from_cputype(arch_flags[0].cputype);
				if(family_arch_flag != NULL)
					family = (enum bool)(family_arch_flag->cpusubtype ==
							arch_flags[0].cpusubtype);
			}
#ifdef OTOOL
			if(ofile.mh->magic == SWAP_LONG(MH_MAGIC)){
				if(SWAP_LONG(ofile.mh->cputype) ==
						arch_flags[i].cputype &&
						(SWAP_LONG(ofile.mh->cpusubtype) ==
						 arch_flags[i].cpusubtype ||
						 family == TRUE)){
					arch_found = TRUE;
				}
			}
			else
#endif /* OTOOL */
				if(ofile.mh->cputype == arch_flags[i].cputype &&
						(ofile.mh->cpusubtype == arch_flags[i].cpusubtype ||
						 family == TRUE)){
					arch_found = TRUE;
				}
				else{
					error("file: %s does not contain architecture: %s",
							ofile.file_name, arch_flags[i].name);
				}
		}
		if(arch_found == FALSE)
			return;
	}
	if(ofile.mh->filetype == MH_DYLIB){
		if(dylib_flat == TRUE){
			processor(&ofile, NULL, cookie);
		}
		else{
			if(member_name != NULL){
				if(ofile_specific_module(member_name, &ofile) == TRUE)
					processor(&ofile, NULL, cookie);
			}
			else{
				/* loop through the dynamic library */
				if(ofile_first_module(&ofile) == TRUE){
					do{
						processor(&ofile, NULL, cookie);
					}while(ofile_next_module(&ofile) == TRUE);
				}
				else{
					error("dynamic library: %s contains no modules",
							ofile.file_name);
				}
			}
		}
	}
	else{
		if(member_name != NULL)
			error("file: %s is not an archive and thus does not contain"
					" member: %s", ofile.file_name, member_name);
		else
			processor(&ofile, NULL, cookie);
	}
}
else{
	if(process_non_objects == TRUE)
		processor(&ofile, NULL, cookie);
	else if(member_name != NULL)
		error("file: %s(%s) is not an object file", name,
				member_name);
	else
		error("file: %s is not an object file", name);
}

