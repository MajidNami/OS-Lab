#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

static LIST_HEAD(birthday_list);
struct birthday *person1 = NULL;
struct birthday *person2 = NULL;
struct birthday *person3 = NULL;
struct birthday *person4 = NULL;
struct birthday *person5 = NULL;

struct birthday 
{
	int day;
	int month;
	int year;
	struct list_head list;
};

int add_birth(void)
{

	

	struct birthday *person = NULL;


	person1 = kmalloc(sizeof(*person), GFP_KERNEL);
	person1->day = 1;
	person1->month = 6;
	person1->year = 1995;
	INIT_LIST_HEAD(&person1->list);
	list_add_tail(&person1->list, &birthday_list);


	person2 = kmalloc(sizeof(*person), GFP_KERNEL);
	person2->day = 2;
	person2->month = 6;
	person2->year = 1995;
	INIT_LIST_HEAD(&person2->list);
	list_add_tail(&person2->list, &birthday_list);

	person3 = kmalloc(sizeof(*person), GFP_KERNEL);
	person3->day = 3;
	person3->month = 6;
	person3->year = 1995;
	INIT_LIST_HEAD(&person3->list);
	list_add_tail(&person3->list, &birthday_list);

	person4 = kmalloc(sizeof(*person), GFP_KERNEL);
	person4->day = 4;
	person4->month = 6;
	person4->year = 1995;
	INIT_LIST_HEAD(&person4->list);
	list_add_tail(&person4->list, &birthday_list);

	person5 = kmalloc(sizeof(*person), GFP_KERNEL);
	person5->day = 5;
	person5->month = 6;
	person5->year = 1995;
	INIT_LIST_HEAD(&person5->list);
	list_add_tail(&person5->list, &birthday_list);


	struct birthday *ptr;
	list_for_each_entry(ptr, &birthday_list, list) {
	/*on each iteration ptr points to the next birthday struct*/

		printk(KERN_INFO "DATA:	%2d/%2d/%4d\n", ptr->day, ptr->month, ptr->year);
	}


	return 0;
}

int remove_birthday(void)
{
	struct birthday *ptr2; //for iterating
	struct birthday *ptr3; // for deleting
	struct birthday *ptr4; // for making the space free

	list_for_each_entry(ptr2, &birthday_list, list) 
	{
		ptr3 = ptr2;
		ptr4 = ptr2;
	}

	printk(KERN_INFO "\n Deleting entry \n");
	list_del(&ptr3->list);
	kfree(ptr4);
	ptr4 = NULL;
	ptr3 = NULL;
	list_for_each_entry(ptr2, &birthday_list, list) 
	{
		printk(KERN_INFO "DATA:	%2d/%2d/%4d\n", ptr2->day, ptr2->month, ptr2->year);
		ptr3 = ptr2;
		ptr4 = ptr2;
	}
	list_del(&ptr3->list);
	kfree(ptr4);
	ptr4 = NULL;
	ptr3 = NULL;
	list_for_each_entry(ptr2, &birthday_list, list) 
	{
		printk(KERN_INFO "DATA:	%2d/%2d/%4d\n", ptr2->day, ptr2->month, ptr2->year);
		ptr3 = ptr2;
		ptr4 = ptr2;
	}
	list_del(&ptr3->list);
	kfree(ptr4);
	ptr4 = NULL;
	ptr3 = NULL;
	list_for_each_entry(ptr2, &birthday_list, list) 
	{
		printk(KERN_INFO "DATA:	%2d/%2d/%4d\n", ptr2->day, ptr2->month, ptr2->year);
		ptr3 = ptr2;
		ptr4 = ptr2;
	}
	list_del(&ptr3->list);
	kfree(ptr4);
	ptr4 = NULL;
	ptr3 = NULL;
	list_for_each_entry(ptr2, &birthday_list, list) 
	{
		printk(KERN_INFO "DATA:	%2d/%2d/%4d\n", ptr2->day, ptr2->month, ptr2->year);
		ptr3 = ptr2;
		ptr4 = ptr2;
	}
	list_del(&ptr3->list);
	kfree(ptr4);
	ptr4 = NULL;
	ptr3 = NULL;
	list_for_each_entry(ptr2, &birthday_list, list) 
	{
		printk(KERN_INFO "DATA:	%2d/%2d/%4d\n", ptr2->day, ptr2->month, ptr2->year);
	}
	
	return 0;
}



/* this function is called when the module is loaded*/
int simple_init(void)
{
	printk(KERN_INFO "Loading Module\n");
	add_birth();
	return 0;
}




/* this function is called when the module is removed*/
void simple_exit(void)
{
	printk(KERN_INFO "Removing Module\n");
	remove_birthday();
}



/* Macros for registering module entry and exit points.
*/
module_init(simple_init);
module_exit(simple_exit);



MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("simple module");
MODULE_AUTHOR("SGG");
