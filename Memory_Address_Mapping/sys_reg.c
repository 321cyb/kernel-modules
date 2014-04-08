 #include <linux/module.h>    
 #include <linux/proc_fs.h>    
 #include <linux/sched.h>
 #include <asm/uaccess.h>
 #include <asm/desc.h>
 #include <asm/pgtable.h>
 #include <asm/desc.h>
 #include <linux/seq_file.h>

 static char modname[] = "sys_reg";
/*
 struct gdtr_struct{
	unsigned short limit;
	unsigned long address;
  }__attribute__((packed));
*/

 static unsigned long cr0,cr3,cr4;
 static struct desc_ptr gdtr;

 static int my_proc_show(struct seq_file *m, void *v)
 {
	struct mm_struct *mm;

	mm = current->active_mm;
	cr0 = read_cr0();
        cr3 = read_cr3();
	cr4 = read_cr4();
//	asm(" sgdt gdtr");
//        asm volatile("sgdt %0":"=m" (gdtr));
        native_store_gdt(&gdtr);


        seq_printf(m, "cr4=%08X  PSE=%X  PAE=%X  \ncr3=%08X cr0=%08X\npgd:0x%08lX\ngdtr address:%lX, limit:%X\n",
        cr4, (cr4>>4)&1 , (cr4>>5)&1 , cr3,cr0, (unsigned long)mm->pgd->pgd, gdtr.address,gdtr.size);
/*
	len += sprintf( buf+len, "cr4=%08X  ", cr4 );
	len += sprintf( buf+len, "PSE=%X  ", (cr4>>4)&1 );
	len += sprintf( buf+len, "PAE=%X  ", (cr4>>5)&1 );
	len += sprintf( buf+len, "\n" );
	len += sprintf( buf+len, "cr3=%08X cr0=%08X\n",cr3,cr0);
	len += sprintf( buf+len, "pgd:0x%08lX\n",(unsigned long)mm->pgd->pgd);
	len += sprintf( buf+len, "gdtr address:%lX, limit:%X\n", gdtr.address,gdtr.limit);
*/
//	len += sprintf( buf+len, "cpu_gdt_table address:0x%08lX\n", cpu_gdt_table);

	return	0;
 }

 static int my_proc_open(struct inode *inode, struct file *file) {
        return single_open(file, my_proc_show, NULL);
 }

 static struct file_operations fops = {
    .open = my_proc_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
 };


 int init_module( void )
 {
	printk( "<1>\nInstalling \'%s\' module\n", modname );
        proc_create_data( modname, 0, NULL, &fops, NULL );
	return	0;  
 }


 void cleanup_module( void )
 {
	remove_proc_entry( modname, NULL );
	printk( "<1>Removing \'%s\' module\n", modname );
 }

 MODULE_LICENSE("GPL");

