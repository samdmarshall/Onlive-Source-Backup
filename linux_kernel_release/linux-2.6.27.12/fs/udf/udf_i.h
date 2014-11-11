#ifndef _UDF_I_H
#define _UDF_I_H

struct udf_inode_info {
	struct timespec		i_crtime;
	/* Physical address of inode */
	kernel_lb_addr		i_location;
	__u64			i_unique;
	__u32			i_lenEAttr;
	__u32			i_lenAlloc;
	__u64			i_lenExtents;
	__u32			i_next_alloc_block;
	__u32			i_next_alloc_goal;
	unsigned		i_alloc_type : 3;
	unsigned		i_efe : 1;	/* extendedFileEntry */
	unsigned		i_use : 1;	/* unallocSpaceEntry */
	unsigned		i_strat4096 : 1;
#ifdef CONFIG_MV88DE3010_BERLIN_UDF_READ12
	unsigned		i_file_type : 3;
	unsigned		reserved : 23;
#else
	unsigned		reserved : 26;
#endif
#ifdef CONFIG_MV88DE3010_BERLIN_UDF_SPEEDUP
	kernel_lb_addr          i_curext;
	unsigned                i_curelen;/* current elen got througth udf_next_aext */
	sector_t                i_curendblock; /* the end block position of file conrresponding to the end of curext */
#endif
	union {
		short_ad	*i_sad;
		long_ad		*i_lad;
		__u8		*i_data;
	} i_ext;
	struct inode vfs_inode;
};

static inline struct udf_inode_info *UDF_I(struct inode *inode)
{
	return list_entry(inode, struct udf_inode_info, vfs_inode);
}

#endif /* _UDF_I_H) */
