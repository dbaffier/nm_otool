#include "woody.h"


/*
** 
*/

int    Mach_O_integrity(char *path)
{
    void                        *file;
    struct stat                 s;
    struct mach_header_64       *mh64;
    struct load_command         *lc;
    struct segment_command_64   *sg64;
    struct section_64           *s64;
    uint64_t                    big_load_end;
    uint64_t                    big_size;
    uint32_t                    sizeofhdr;
    int                         fd;


    if (stat(path, &s) == -1)
        return (ft_fatal("stat error"));
    if ((fd = open(path, O_RDONLY)) < 0)
        return (ft_fatal("open error"));
    file = mmap(NULL, s.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    close(fd);
    if (file == MAP_FAILED)
        return (ft_fatal("mmap failed"));
    uint32_t magic;

    magic = *((unsigned int *)file);

    if (!is_Macho(*(unsigned int *)file))
        return (ft_fatal("Not an Mach-o file"));
    if (!Macho_64(magic))
        return (ft_fatal("Not an x64 Mach-o file"));
    mh64 = (struct mach_header_64 *)file;
    if (mh64 == NULL)
        return (ft_fatal("Not an x64 Mach-O header"));
    if (mh64->sizeofcmds + sizeof(struct mach_header_64) > (unsigned long)s.st_size)
        return (ft_fatal("sizeofcmds extend past the size of the file"));
    sizeofhdr = mh64->sizeofcmds + sizeof(struct mach_header_64);
    lc = (struct load_command *)(file + sizeof(struct mach_header_64));
    big_load_end = 0;
    for (uint32_t i = 0; i < mh64->ncmds; i++)
    {
        if (big_load_end + sizeof(struct load_command) > mh64->sizeofcmds)
            return (ft_fatal("error load_command"));
        if (lc->cmdsize % 8 != 0)
            return (ft_fatal("cmdsize is not an multiple of 8"));

        big_load_end += lc->cmdsize;
        if (big_load_end > (unsigned long)s.st_size)
            return (ft_fatal("cmd size extend past the size of the file"));
        if (lc->cmdsize == 0)
            return (ft_fatal("cmdsize is 0"));
        if (lc->cmd == LC_SEGMENT_64)
        {
            if (lc->cmdsize < sizeof(struct segment_command_64))
                return (ft_fatal("cmdsize is lower than segment_command struct"));
            sg64 = (struct segment_command_64 *)lc;
            big_size = sg64->nsects;
            big_size *= sizeof(struct section_64);
            big_size += sizeof(struct segment_command_64);
            if (sg64->cmdsize != big_size)
                return (ft_fatal("cmdsize doesn't match is real size"));
            if (sg64->fileoff > (unsigned long)s.st_size)
                return (ft_fatal("fileoff extend past the size of the file"));
            big_size = sg64->fileoff;
            big_size += sg64->filesize;
            if (big_size > (unsigned long)s.st_size)
                return (ft_fatal("fileoff + filesize extend past the size of the file "));
            s64 = (struct section_64 *)((char *)sg64 + sizeof(struct segment_command_64));
            for (uint32_t j = 0; j < sg64->nsects; j++)
            {
                if (sg64->fileoff == 0 && s64->offset < sizeofhdr && s64->size != 0)
                    return (ft_fatal("section error"));
                big_size = s64->offset;
                big_size += s64->size;
                if (big_size > (unsigned long)s.st_size)
                    return (ft_fatal("section offset + size extend past the size of the file"));
                if (s64->reloff > s.st_size)
                    return (ft_fatal("section reloff extend past the size of the file"));
            }
        }
        lc = (struct load_command *)((char *)lc + lc->cmdsize);
    }
    munmap(file, s.st_size);
    return (1);
}

int		is_Macho(uint32_t magic)
{
	return (magic == MH_MAGIC || magic == MH_CIGAM ||
			magic == MH_MAGIC_64 || magic == MH_CIGAM_64);
}	

int		Macho_64(uint32_t magic)
{
	return (magic == MH_MAGIC_64 || magic == MH_CIGAM_64);
}

/*
** Return file offset to last cmp
*/

size_t file_offset_lsegment(char *file, uint32_t cmp)
{
    struct mach_header_64 *mh64;
    struct load_command *lc;
    struct segment_command_64 *sc;

    mh64 = (struct mach_header_64 *)file;
    lc = (struct load_command *)(file + sizeof(struct mach_header_64));
    for (size_t i = 0; i < mh64->ncmds; i++)
    {
        if (lc->cmd == cmp)
            sc = (struct segment_command_64 *)lc;
        lc = (struct load_command *)((char *)lc + lc->cmdsize);
    }
    return ((size_t)sc - (size_t)file);
}

/*
** Return offset to the end of last LC_SEGMENT_64 from *file
*/

size_t file_offset_end_lsegment(char *file, uint32_t cmp)
{
    struct mach_header_64 *mh64;
    struct load_command *lc;
    struct segment_command_64 *sc;

    mh64 = (struct mach_header_64 *)file;
    lc = (struct load_command *)(file + sizeof(struct mach_header_64));
    for (size_t i = 0; i < mh64->ncmds; i++)
    {
        if (lc->cmd == cmp)
            sc = (struct segment_command_64 *)lc;
        lc = (struct load_command *)((char *)lc + lc->cmdsize);
    }
    return ((((size_t)sc + (size_t)sc->cmdsize)) - (size_t)file);
}

/*
** Return entry offset (__TEXT of main)
** Work only for MH_EXECUTE
*/

uint64_t entry_offset(char *file)
{
    struct mach_header_64 *mh64;
    struct load_command *lc;

    mh64 = (struct mach_header_64 *)file;
    lc = (struct load_command *)(file + sizeof(struct mach_header_64));
    for (size_t i = 0; i < mh64->ncmds; i++)
    {
        if (lc->cmd == LC_MAIN)
            return (((struct entry_point_command *)lc)->entryoff);
        lc = (struct load_command *)((char *)lc + lc->cmdsize);
    }
    ft_fatal("entryoff not found");
    return (0);
}

/*
** Return file data offset to the end of lc(segment)
** -> new entryoff
*/

uint64_t    last_seg_offset_end(char *file, uint32_t cmp)
{
    struct mach_header_64       *mh64;
    struct load_command         *lc;
    struct segment_command_64   *sc;

    mh64 = (struct mach_header_64 *)file;
    lc = (struct load_command *)(file + sizeof(struct mach_header_64));
    for (size_t i = 0; i < mh64->ncmds; i++)
    {
        if (lc->cmd == cmp)
            sc = (struct segment_command_64 *)lc;
        lc = (struct load_command *)((char *)lc + lc->cmdsize);
    }
    return (sc->fileoff + sc->vmsize);
}

/*
** Return vmaddr offset to the end of lc(segment)
*/

uint64_t last_seg_vmoffset_end(char *file, uint32_t cmp)
{
    struct mach_header_64       *mh64;
    struct load_command         *lc;
    struct segment_command_64   *sc;

    mh64 = (struct mach_header_64 *)file;
    lc = (struct load_command *)(file + sizeof(struct mach_header_64));
    for (size_t i = 0; i < mh64->ncmds; i++)
    {
        if (lc->cmd == cmp)
            sc = (struct segment_command_64 *)lc;
        lc = (struct load_command *)((char *)lc + lc->cmdsize);
    }
    return (sc->vmaddr + sc->vmsize);
}

/*
** Patch entryoff
** Return old entryoff - size of new entry (since file will be bigger)
*/

uint64_t        patch_entryoff(char *file, uint64_t new)
{
    struct mach_header_64   *mh64;
    struct load_command     *lc;
    uint64_t                old;

    mh64 = (struct mach_header_64 *)file;
    lc = (struct load_command *)(file + sizeof(struct mach_header_64));
    old = entry_offset(file);
    for (size_t i = 0; i < mh64->ncmds; i++)
    {
        if (lc->cmd == LC_MAIN)
            ((struct entry_point_command *)lc)->entryoff = new;
        lc = (struct load_command *)((char *)lc + lc->cmdsize);
    }
    return (old - new);
}

/*
** Increase segment and size by 1
** Disable ASLR
*/

void patch_macho64_header(char *file)
{
    struct mach_header_64 *mh64;

    mh64 = (struct mach_header_64 *)file;

    mh64->ncmds += 1;
    mh64->sizeofcmds += (sizeof(struct segment_command_64) + sizeof(struct section_64));
    /* disable ASLR (random space address) */
    mh64->filetype &= ~MH_PIE;
}

/*
** Find segment by cmp
*/

struct segment_command_64 *find_segment(char *file, char *cmp)
{
    struct mach_header_64   *mh64;
    struct load_command     *lc;

    mh64 = (struct mach_header_64 *)file;
    lc = (struct load_command *)(file + sizeof(struct mach_header_64));
    for (size_t i = 0; i < mh64->ncmds; i++)
    {
        if (lc->cmd == LC_SEGMENT_64)
        {
            if (ft_strcmp(((struct segment_command_64 *)lc)->segname, cmp) == 0)
                return ((struct segment_command_64 *)lc);
        }
        lc = (struct load_command *)((char *)lc + lc->cmdsize);
    }
    return (NULL);
}

/*
** Return file offset of section __text
*/

uint32_t    find_sectext(char *file)
{
    struct mach_header_64   *mh64;
    struct load_command     *lc;
    struct section_64       *section64;


    mh64 = (struct mach_header_64 *)file;
    lc = (struct load_command *)(file + sizeof(struct mach_header_64));
    for (size_t i = 0; i < mh64->ncmds; i++)
    {
        if (lc->cmd == LC_SEGMENT_64)
        {
            if (ft_strcmp(((struct segment_command_64 *)lc)->segname, SEG_TEXT) == 0)
            {
                section64 = (struct section_64 *)((char *)lc + sizeof(struct segment_command_64));
                for (size_t j = 0; j < ((struct segment_command_64 *)lc)->nsects; j++)
                {
                    if (ft_strcmp((section64 + j)->sectname, SECT_TEXT) == 0)
                        return ((size_t)(section64 + j) - (size_t)file);

                }
            }
        }
        lc = (struct load_command *)((char *)lc + lc->cmdsize);
    }
    return (0);
}

/*
** Create new segment
** By default contain 1 section.
** By default protection are applied to both maxprot and initprot
*/

struct segment_command_64 create_segment(char segname[16], uint32_t cmd, vm_prot_t protection)
{
    struct segment_command_64       segment;

    segment.cmd = cmd;
    ft_strncpy(segment.segname, segname, 16);
    segment.cmdsize = sizeof(struct segment_command_64) + sizeof(struct section_64);
    segment.vmaddr = 0;
    segment.vmsize = 0;
    segment.fileoff = 0;
    segment.filesize = 0;
    segment.maxprot = protection;
    segment.initprot = protection;
    segment.nsects = 1;
    segment.flags = 0;

    return (segment);
}

/*
** Create new section
*/

struct section_64 create_section(char sectname[16])
{
    struct section_64 section;

    ft_memset(&section, 0, sizeof(struct section_64));
    ft_strncpy(section.sectname, sectname, 16);
    
    return (section);
}