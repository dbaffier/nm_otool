static
void
processor(
struct ofile *ofile,
char *arch_name,
void *cookie) /* cookie is not used */
{
    char *addr;
    unsigned long i, size, magic;
    struct mach_header mh;
    struct load_command *load_commands;
    unsigned long nsymbols, nsorted_symbols, strings_size, len;
    struct nlist *symbols, *allocated_symbols, *sorted_symbols;
    char *strings, *p;
    unsigned char n_type;
    char *sect;
    unsigned long sect_size, sect_addr, sect_nrelocs, sect_flags, nrelocs;
    struct relocation_info *sect_relocs, *relocs;
    unsigned long *indirect_symbols, *allocated_indirect_symbols,
		  nindirect_symbols;
    struct dylib_module *mods, *allocated_mods;
    struct dylib_table_of_contents *tocs, *allocated_tocs;
    struct dylib_reference *refs, *allocated_refs;
    unsigned long nmods, ntocs, nrefs;
    struct twolevel_hint *hints, *allocated_hints;
    unsigned long nhints;

	sorted_symbols = NULL;
	nsorted_symbols = 0;
	indirect_symbols = NULL;
	nindirect_symbols = 0;
	hints = NULL;
	nhints = 0;
	/*
	 * These may or may not be allocated.  If allocated they will not be
	 * NULL and then free'ed before returning.
	 */
	load_commands = NULL;
	allocated_symbols = NULL;
	sorted_symbols = NULL;
	allocated_indirect_symbols = NULL;
	allocated_tocs = NULL;
	allocated_mods = NULL;
	allocated_refs = NULL;
	allocated_hints = NULL;

	/*
	 * The fat headers are printed in ofile_map() in ofile.c #ifdef'ed
	 * OTOOL.
	 */

	/*
	 * Archive headers.
	 */
	if(aflag && ofile->member_ar_hdr != NULL)
	    print_ar_hdr(ofile->member_ar_hdr, ofile->member_name,
			 ofile->member_name_size, vflag);

	/*
	 * Archive table of contents.
	 */
	if(ofile->member_ar_hdr != NULL &&
	   strncmp(ofile->member_name, SYMDEF, sizeof(SYMDEF)-1) == 0){
	    if(Sflag == FALSE)
		return;
	    if(ofile->file_type == OFILE_FAT){
		addr = ofile->file_addr + ofile->fat_archs[ofile->narch].offset;
		size = ofile->fat_archs[ofile->narch].size;
	    }
	    else{
		addr = ofile->file_addr;
		size = ofile->file_size;
	    }
	    if(addr + size > ofile->file_addr + ofile->file_size)
		size = (ofile->file_addr + ofile->file_size) - addr;
	    print_library_toc(ofile->member_ar_hdr, /* toc_ar_hdr */
			      ofile->member_name, /* toc_name */
			      ofile->member_name_size, /* toc_name_size */
			      ofile->member_addr, /* toc_addr */
			      ofile->member_size, /* toc_size */
			      get_toc_byte_sex(addr, size),
			      ofile->file_name, /* library_name */
			      addr, /* library_addr */
			      size, /* library_size */
			      arch_name,
			      vflag);
	    return;
	}

	if(object_processing == FALSE)
	    return;

	/*
	 * Print header for the object name if in an archive or an architecture
	 * name is passed in.
	 */
	if(Xflag == FALSE){
	    printf("%s", ofile->file_name);
	    if(ofile->member_ar_hdr != NULL){
		printf("(%.*s)", (int)ofile->member_name_size,
			ofile->member_name);
	    }
	    if(arch_name != NULL)
		printf(" (architecture %s):", arch_name);
	    else
		printf(":");
	    /*
	     * If the mach_header pointer is NULL the file is not an object
	     * file.  Truncated object file (where the file size is less
	     * than sizeof(struct mach_header)) also does not have it's
	     * mach_header set.  So deal with both cases here and then
	     * return as the rest of this routine deals only with things
	     * in object files.
	     */
	    if(ofile->mh == NULL){
		if(ofile->file_type == OFILE_FAT){
		    /*
		     * This routine is not called on fat files where the
		     * offset is past end of file.  An error message is
		     * printed in ofile_specific_arch() in ofile.c.
		     */
		    if(ofile->arch_type == OFILE_ARCHIVE){
			addr = ofile->member_addr;
			size = ofile->member_size;
		    }
		    else{
			addr = ofile->file_addr +
			       ofile->fat_archs[ofile->narch].offset;
			size = ofile->fat_archs[ofile->narch].size;
		    }
		    if(addr + size > ofile->file_addr + ofile->file_size)
			size = (ofile->file_addr + ofile->file_size) - addr;
		}
		else if(ofile->file_type == OFILE_ARCHIVE){
		    addr = ofile->member_addr;
		    size = ofile->member_size;
		}
		else{ /* ofile->file_type == OFILE_UNKNOWN */
		    addr = ofile->file_addr;
		    size = ofile->file_size;
		}
		if(size > sizeof(long)){
		    memcpy(&magic, addr, sizeof(unsigned long));
		    if(magic == MH_MAGIC || magic == SWAP_LONG(MH_MAGIC)){
			printf(" is a truncated object file\n");
			memset(&mh, '\0', sizeof(struct mach_header));
			if(size > sizeof(struct mach_header))
			    size = sizeof(struct mach_header);
			memcpy(&mh, addr, size);
			if(magic == SWAP_LONG(MH_MAGIC))
			    swap_mach_header(&mh, get_host_byte_sex());
			if(hflag)
			    print_mach_header(&mh, vflag);
			return;
		    }
		}
		printf(" is not an object file\n");
		return;
	    }
	}
	if(ofile->mh != NULL){
	    if((int)(ofile->mh) % sizeof(unsigned long)){
		if(Xflag == FALSE)
		    printf("(object file offset is not a multiple of sizeof("
			   "unsigned long))");
		memcpy(&mh, ofile->mh, sizeof(struct mach_header));
		if(mh.magic == SWAP_LONG(MH_MAGIC))
		    swap_mach_header(&mh, get_host_byte_sex());
		ofile->mh = &mh;
	    }
	    else if(ofile->mh->magic == SWAP_LONG(MH_MAGIC)){
		mh = *(ofile->mh);
		swap_mach_header(&mh, get_host_byte_sex());
		ofile->mh = &mh;
	    }
	}
	if(Xflag == FALSE)
	    printf("\n");

	/*
	 * If this is not an object file then just return.
	 */
	if(ofile->mh == NULL)
	    return;

	/*
	 * Calculate the true number of bytes of the of the object file that
	 * is in memory (in case this file is truncated).
	 */
	addr = ofile->object_addr;
	size = ofile->object_size;
	if(addr + size > ofile->file_addr + ofile->file_size)
	    size = (ofile->file_addr + ofile->file_size) - addr;

	/*
	 * Mach header.
	 */
	if(hflag)
	    print_mach_header(ofile->mh, vflag);

	/*
	 * Load commands.
	 */
	if(ofile->mh->sizeofcmds + sizeof(struct mach_header) > size){
	    load_commands = allocate(ofile->mh->sizeofcmds);
	    memset(load_commands, '\0', ofile->mh->sizeofcmds);
	    memcpy(load_commands, ofile->load_commands, 
		   size - sizeof(struct mach_header));
	    ofile->load_commands = load_commands;
	}
	if(lflag)
	    print_loadcmds(ofile->mh, ofile->load_commands,
			   ofile->object_byte_sex, size, vflag, Vflag);

	if(Lflag || Dflag)
	    print_libraries(ofile->mh, ofile->load_commands,
			    ofile->object_byte_sex, (Dflag && !Lflag), vflag);

	/*
	 * If the indicated operation needs the symbol table get it.
	 */
	sect_flags = 0;
	if(segname != NULL && sectname != NULL)
	    (void)get_sect_info(segname, sectname, ofile->mh,
			ofile->load_commands, ofile->object_byte_sex,
			addr, size, &sect, &sect_size, &sect_addr,
			&sect_relocs, &sect_nrelocs, &sect_flags);
	if(Rflag || Mflag)
	    get_symbol_table_info(ofile->mh, ofile->load_commands,
		ofile->object_byte_sex, addr, size,
		&symbols, &nsymbols, &strings, &strings_size);
	if(vflag && (rflag || Tflag || Mflag || Rflag || Iflag || Hflag || tflag
	   || iflag || oflag ||
	   (sect_flags & SECTION_TYPE) == S_LITERAL_POINTERS ||
	   (sect_flags & S_ATTR_PURE_INSTRUCTIONS) ==
		S_ATTR_PURE_INSTRUCTIONS ||
	   (sect_flags & S_ATTR_SOME_INSTRUCTIONS) ==
		S_ATTR_SOME_INSTRUCTIONS)){
	    get_symbol_table_info(ofile->mh, ofile->load_commands,
		ofile->object_byte_sex, addr, size,
		&symbols, &nsymbols, &strings, &strings_size);

	    if((int)symbols % sizeof(unsigned long) ||
	       ofile->object_byte_sex != get_host_byte_sex()){
		allocated_symbols = allocate(nsymbols * sizeof(struct nlist));
		memcpy(allocated_symbols, symbols,
		       nsymbols * sizeof(struct nlist));
		symbols = allocated_symbols;
	    }
	    if(ofile->object_byte_sex != get_host_byte_sex())
		swap_nlist(symbols, nsymbols, get_host_byte_sex());

	    /*
	     * If the operation needs a sorted symbol table create it.
	     */
	    if(tflag || iflag || oflag || 
	       (sect_flags & S_ATTR_PURE_INSTRUCTIONS) ==
		    S_ATTR_PURE_INSTRUCTIONS ||
	       (sect_flags & S_ATTR_SOME_INSTRUCTIONS) ==
		    S_ATTR_SOME_INSTRUCTIONS){
		sorted_symbols = allocate(nsymbols * sizeof(struct nlist));
		nsorted_symbols = 0;
		for(i = 0; i < nsymbols; i++){
		    if(symbols[i].n_un.n_strx > 0 &&
			symbols[i].n_un.n_strx < strings_size)
			p = strings + symbols[i].n_un.n_strx;
		    else
			p = "symbol with bad string index";
		    if(symbols[i].n_type & ~(N_TYPE|N_EXT|N_PEXT))
			continue;
		    n_type = symbols[i].n_type & N_TYPE;
		    if(n_type == N_ABS || n_type == N_SECT){
			len = strlen(p);
			if(len > sizeof(".o") - 1 &&
			   strcmp(p + (len - (sizeof(".o") - 1)), ".o") == 0)
			    continue;
			if(strcmp(p, "gcc_compiled.") == 0)
			    continue;
			if(n_type == N_ABS && symbols[i].n_value == 0 &&
			   *p == '.')
			    continue;
			sorted_symbols[nsorted_symbols] = symbols[i];
			sorted_symbols[nsorted_symbols].n_un.n_name = p;
			nsorted_symbols++;
		    }
		}
		qsort(sorted_symbols, nsorted_symbols, sizeof(struct nlist),
		      (int (*)(const void *, const void *))sym_compare);
	    }
	}

	if(Mflag || Tflag || Rflag){
	    get_module_table_info(ofile->mh, ofile->load_commands,
		ofile->object_byte_sex, addr, size, &mods, &nmods);
	    if((int)mods % sizeof(unsigned long) ||
	       ofile->object_byte_sex != get_host_byte_sex()){
		allocated_mods = allocate(nmods * sizeof(struct dylib_module));
		memcpy(allocated_mods, mods,
		       nmods * sizeof(struct dylib_module));
		mods = allocated_mods;
	    }
	    if(ofile->object_byte_sex != get_host_byte_sex())
		swap_dylib_module(mods, nmods, get_host_byte_sex());
	}

	if(Tflag){
	    get_toc_info(ofile->mh, ofile->load_commands,
		ofile->object_byte_sex, addr, size, &tocs, &ntocs);
	    if((int)tocs % sizeof(unsigned long) ||
	       ofile->object_byte_sex != get_host_byte_sex()){
		allocated_tocs = allocate(ntocs *
					sizeof(struct dylib_table_of_contents));
		memcpy(allocated_tocs, tocs,
		       ntocs * sizeof(struct dylib_table_of_contents));
		tocs = allocated_tocs;
	    }
	    if(ofile->object_byte_sex != get_host_byte_sex())
		swap_dylib_table_of_contents(tocs, ntocs, get_host_byte_sex());
	    print_toc(ofile->mh, ofile->load_commands,
		ofile->object_byte_sex, addr, size, tocs, ntocs, mods, nmods,
		symbols, nsymbols, strings, strings_size, vflag);
	}

	if(Mflag)
	    print_module_table(ofile->mh, ofile->load_commands,
		ofile->object_byte_sex, addr, size, mods, nmods,
		symbols, nsymbols, strings, strings_size, vflag);

	if(Rflag){
	    get_ref_info(ofile->mh, ofile->load_commands,
		ofile->object_byte_sex, addr, size, &refs, &nrefs);
	    if((int)refs % sizeof(unsigned long) ||
	       ofile->object_byte_sex != get_host_byte_sex()){
		allocated_refs = allocate(nrefs *
					sizeof(struct dylib_reference));
		memcpy(allocated_refs, refs,
		       nrefs * sizeof(struct dylib_reference));
		refs = allocated_refs;
	    }
	    if(ofile->object_byte_sex != get_host_byte_sex())
		swap_dylib_reference(refs, nrefs, get_host_byte_sex());
	    print_refs(ofile->mh, ofile->load_commands,
		ofile->object_byte_sex, addr, size, refs, nrefs, mods, nmods,
		symbols, nsymbols, strings, strings_size, vflag);
	}

	if(Iflag || (tflag && vflag)){
	    get_indirect_symbol_table_info(ofile->mh, ofile->load_commands,
		ofile->object_byte_sex, addr, size,
		&indirect_symbols, &nindirect_symbols);
	    if((int)indirect_symbols % sizeof(unsigned long) ||
	       ofile->object_byte_sex != get_host_byte_sex()){
		allocated_indirect_symbols = allocate(nindirect_symbols *
						     sizeof(unsigned long));
		memcpy(allocated_indirect_symbols, indirect_symbols,
		       nindirect_symbols * sizeof(unsigned long));
		indirect_symbols = allocated_indirect_symbols;
	    }
	    if(ofile->object_byte_sex != get_host_byte_sex())
		swap_indirect_symbols(indirect_symbols, nindirect_symbols,
				      get_host_byte_sex());
	}
	if(Hflag){
	    get_hints_table_info(ofile->mh, ofile->load_commands,
		ofile->object_byte_sex, addr, size,
		&hints, &nhints);
	    if((int)hints % sizeof(unsigned long) ||
	       ofile->object_byte_sex != get_host_byte_sex()){
		allocated_hints = allocate(nhints *
					   sizeof(struct twolevel_hint));
		memcpy(allocated_hints, hints,
		       nhints * sizeof(struct twolevel_hint));
		hints = allocated_hints;
	    }
	    if(ofile->object_byte_sex != get_host_byte_sex())
		swap_twolevel_hint(hints, nhints, get_host_byte_sex());
	    print_hints(ofile->mh, ofile->load_commands,
		ofile->object_byte_sex, addr, size, hints,
		nhints, symbols, nsymbols, strings, strings_size, vflag);
	}
	if(Iflag)
	    print_indirect_symbols(ofile->mh, ofile->load_commands,
		ofile->object_byte_sex, addr, size, indirect_symbols,
		nindirect_symbols, symbols, nsymbols, strings, strings_size,
		vflag);

	if(rflag)
	    print_reloc(ofile->mh, ofile->load_commands, ofile->object_byte_sex,
			addr, size, symbols, nsymbols, strings, strings_size,
			vflag);

	if(tflag ||
	   (sect_flags & S_ATTR_PURE_INSTRUCTIONS) ==
		S_ATTR_PURE_INSTRUCTIONS ||
	   (sect_flags & S_ATTR_SOME_INSTRUCTIONS) ==
		S_ATTR_SOME_INSTRUCTIONS){
	    if(tflag)
		(void)get_sect_info(SEG_TEXT, SECT_TEXT, ofile->mh,
		    ofile->load_commands, ofile->object_byte_sex,
		    addr, size, &sect, &sect_size, &sect_addr,
		    &sect_relocs, &sect_nrelocs, &sect_flags);

	    /* create aligned relocations entries as needed */
	    relocs = NULL;
	    nrelocs = 0;
	    if(Vflag){
		if((long)sect_relocs % sizeof(long) != 0 ||
		   ofile->object_byte_sex != get_host_byte_sex()){
		    nrelocs = sect_nrelocs;
		    relocs = allocate(nrelocs *
				      sizeof(struct relocation_info));
		    memcpy(relocs, sect_relocs, nrelocs *
			   sizeof(struct relocation_info));
		}
		else{
		    nrelocs = sect_nrelocs;
		    relocs = sect_relocs;
		}
		if(ofile->object_byte_sex != get_host_byte_sex())
		    swap_relocation_info(relocs, nrelocs,
					 get_host_byte_sex());
	    }
	    if(Xflag == FALSE){
		if(tflag)
		    printf("(%s,%s) section\n", SEG_TEXT, SECT_TEXT);
		else
		    printf("Contents of (%.16s,%.16s) section\n", segname,
			   sectname);
	    }
	    print_text(ofile->mh->cputype, ofile->object_byte_sex,
		       sect, sect_size, sect_addr, sorted_symbols,
		       nsorted_symbols, symbols, nsymbols, strings,
		       strings_size, relocs, nrelocs,
indirect_symbols, nindirect_symbols, ofile->mh, ofile->load_commands,
vflag, Vflag);

	    if(relocs != NULL && relocs != sect_relocs)
		free(relocs);
	}

	if(iflag){
	    if(get_sect_info(SEG_TEXT, SECT_FVMLIB_INIT0, ofile->mh,
		ofile->load_commands, ofile->object_byte_sex,
		addr, size, &sect, &sect_size, &sect_addr,
		&sect_relocs, &sect_nrelocs, &sect_flags) == TRUE){

		/* create aligned, sorted relocations entries */
		nrelocs = sect_nrelocs;
		relocs = allocate(nrelocs * sizeof(struct relocation_info));
		memcpy(relocs, sect_relocs, nrelocs *
		       sizeof(struct relocation_info));
		if(ofile->object_byte_sex != get_host_byte_sex())
		    swap_relocation_info(relocs, nrelocs, get_host_byte_sex());
		qsort(relocs, nrelocs, sizeof(struct relocation_info),
		      (int (*)(const void *, const void *))rel_compare);

		if(Xflag == FALSE)
		    printf("Shared library initialization (%s,%s) section\n",
			   SEG_TEXT, SECT_FVMLIB_INIT0);
		print_shlib_init(ofile->object_byte_sex, sect, sect_size,
			sect_addr, sorted_symbols, nsorted_symbols, symbols,
			nsymbols, strings, strings_size, relocs, nrelocs,vflag);
		free(relocs);
	    }
	}

	if(dflag){
	    if(get_sect_info(SEG_DATA, SECT_DATA, ofile->mh,
		ofile->load_commands, ofile->object_byte_sex,
		addr, size, &sect, &sect_size, &sect_addr,
		&sect_relocs, &sect_nrelocs, &sect_flags) == TRUE){

		if(Xflag == FALSE)
		    printf("(%s,%s) section\n", SEG_DATA, SECT_DATA);
		print_sect(ofile->mh->cputype, ofile->object_byte_sex,
			   sect, sect_size, sect_addr);
	    }
	}

	if(segname != NULL && sectname != NULL &&
	   (sect_flags & S_ATTR_PURE_INSTRUCTIONS) !=
		S_ATTR_PURE_INSTRUCTIONS &&
	   (sect_flags & S_ATTR_SOME_INSTRUCTIONS) !=
		S_ATTR_SOME_INSTRUCTIONS){
	    if(strcmp(segname, SEG_OBJC) == 0 &&
	       strcmp(sectname, "__protocol") == 0 && vflag == TRUE){
		print_objc_protocol_section(ofile->mh, ofile->load_commands,
		   ofile->object_byte_sex, ofile->object_addr,
		   ofile->object_size, vflag);
	    }
	    else if(strcmp(segname, SEG_OBJC) == 0 &&
	            (strcmp(sectname, "__string_object") == 0 ||
	             strcmp(sectname, "__cstring_object") == 0) &&
		    vflag == TRUE){
		print_objc_string_object_section(sectname, ofile->mh,
		   ofile->load_commands, ofile->object_byte_sex,
		   ofile->object_addr, ofile->object_size, vflag);
	    }
	    else if(strcmp(segname, SEG_OBJC) == 0 &&
	       strcmp(sectname, "__runtime_setup") == 0 && vflag == TRUE){
		print_objc_runtime_setup_section(ofile->mh,ofile->load_commands,
		   ofile->object_byte_sex, ofile->object_addr,
		   ofile->object_size, vflag);
	    }
	    else if(get_sect_info(segname, sectname, ofile->mh,
		ofile->load_commands, ofile->object_byte_sex,
		addr, size, &sect, &sect_size, &sect_addr,
		&sect_relocs, &sect_nrelocs, &sect_flags) == TRUE){

		if(Xflag == FALSE)
		    printf("Contents of (%.16s,%.16s) section\n", segname,
			   sectname);

		if(vflag){
		    switch((sect_flags & SECTION_TYPE)){
		    case 0:
			print_sect(ofile->mh->cputype, ofile->object_byte_sex,
				   sect, sect_size, sect_addr);
			break;
		    case S_ZEROFILL:
			printf("zerofill section and has no contents in the "
			       "file\n");
			break;
		    case S_CSTRING_LITERALS:
			print_cstring_section(sect, sect_size, sect_addr,
				Xflag == TRUE ? FALSE : TRUE);
			break;
		    case S_4BYTE_LITERALS:
			print_literal4_section(sect, sect_size, sect_addr,
					      ofile->object_byte_sex,
					      Xflag == TRUE ? FALSE : TRUE);
			break;
		    case S_8BYTE_LITERALS:
			print_literal8_section(sect, sect_size, sect_addr,
					      ofile->object_byte_sex,
					      Xflag == TRUE ? FALSE : TRUE);
			break;
		    case S_LITERAL_POINTERS:
			/* create aligned, sorted relocations entries */
			nrelocs = sect_nrelocs;
			relocs = allocate(nrelocs *
					  sizeof(struct relocation_info));
			memcpy(relocs, sect_relocs, nrelocs *
			       sizeof(struct relocation_info));
			if(ofile->object_byte_sex != get_host_byte_sex())
			    swap_relocation_info(relocs, nrelocs,
					         get_host_byte_sex());
			qsort(relocs, nrelocs, sizeof(struct relocation_info),
			      (int (*)(const void *, const void *))rel_compare);
			print_literal_pointer_section(ofile->mh,
				ofile->load_commands, ofile->object_byte_sex,
				addr, size, sect, sect_size, sect_addr,
			   	symbols, nsymbols, strings, strings_size,
				relocs, nrelocs, Xflag == TRUE ? FALSE : TRUE);
			free(relocs);

			break;
		    default:
			printf("Unknown section type (0x%x)\n",
			       (unsigned int)(sect_flags & SECTION_TYPE));
			print_sect(ofile->mh->cputype, ofile->object_byte_sex,
				   sect, sect_size, sect_addr);
			break;
		    }
		}
		else{
		    if((sect_flags & SECTION_TYPE) == S_ZEROFILL)
			printf("zerofill section and has no contents in the "
			       "file\n");
		    else
			print_sect(ofile->mh->cputype, ofile->object_byte_sex,
				   sect, sect_size, sect_addr);
		}
	    }
	}

	if(cflag)
	    print_argstrings(ofile->mh, ofile->load_commands,
			     ofile->object_byte_sex, ofile->object_addr,
			     ofile->object_size);

	if(oflag)
	    print_objc_segment(ofile->mh, ofile->load_commands,
			       ofile->object_byte_sex, ofile->object_addr,
			       ofile->object_size, sorted_symbols,
			       nsorted_symbols, vflag);

	if(load_commands != NULL)
	    free(load_commands);
	if(allocated_symbols != NULL)
	    free(allocated_symbols);
	if(sorted_symbols != NULL)
	    free(sorted_symbols);
	if(allocated_indirect_symbols != NULL)
	    free(allocated_indirect_symbols);
	if(allocated_hints != NULL)
	    free(allocated_hints);
	if(allocated_tocs != NULL)
	    free(allocated_tocs);
	if(allocated_mods != NULL)
	    free(allocated_mods);
	if(allocated_refs != NULL)
	    free(allocated_refs);
}

