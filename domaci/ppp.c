#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/device.h>

#define BUFF_SIZE 200

MODULE_LICENSE("Dual BSD/GPL");

dev_t my_dev_id;
static struct class *my_class;
static struct device *my_device;
static struct cdev *my_cdev;
char ppp[100];
int endRead = 0; 



int ppp_open(struct inode *pinode, struct file *pfile);
int ppp_close(struct inode *pinode, struct file *pfile);
ssize_t ppp_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset);
ssize_t ppp_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset);

struct file_operations my_fops =
{
	.owner = THIS_MODULE,
	.open = ppp_open,
	.read = ppp_read,
	.write = ppp_write,
	.release = ppp_close,
};


int ppp_open(struct inode *pinode, struct file *pfile) 
{
		printk(KERN_INFO "Succesfully opened ppp\n");
		return 0;
}

int ppp_close(struct inode *pinode, struct file *pfile) 
{
		printk(KERN_INFO "Succesfully closed ppp\n");
		return 0;
}

ssize_t ppp_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
	
{
	int ret;
	int pos;
	char buff[BUFF_SIZE];
	long int len=0;
	if (endRead){
         endRead = 0;   //signalizira da se doslo do krajnjeg elementa
		pos = 0;                //pokazuje element na koji se treba vratiti u trenutnom pozivu cat komande
		//printk(KERN_INFO ​"Succesfully read from file\n"​);
		return 0;
    }
	
 
	strcpy(buff, ppp);     //kopira ppp u buff
	ret = copy_to_user(buffer, ppp, strlen(ppp));  //niz ppp se kopira u niz buffer 
	if(ret)                                            //strlen racuna duzinu stringa
	{
		return -EFAULT;
	}
	printk(KERN_WARNING "Succesfully read\n");
	endRead=1;
    	for(pos=0; pos<100; pos++)
		{
			len = scnprintf(buff, BUFF_SIZE, "%c ", ppp[pos]);
			ret = copy_to_user(buffer, buff,len);
			if(ret)
				return -EFAULT;
		}
		if(pos==100)
		{
		//	printk(KERN_INFO ​"Succesfully read from file\n"​);
			endRead = 1;
		
		}
	    else
	    {
			printk(KERN_WARNING "Lifo is empty\n"); 
	    }

	  return strlen(ppp);
}


ssize_t ppp_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset) 
{
	char buff[BUFF_SIZE];
	char bufff[100];
	int ret;
	int j=0;
	int position, value;
	int duzina, duz;
	char str1[90];
	char *shrrink;
	int s;

	ret = copy_from_user(buff, buffer, length);  //kopiramo iz niza buffer u niz buff (kernel prostor)
	if(ret)
	  return -EFAULT;
	
	buff[length-1] = '\0';  //kako bismo mogli koristiti niz karakter buff kao string u pomocnim f-jama
	
		
    if(length<110)
    {                               //upis stringa  //strcmp — Compare two strings  //int strcmp (	const char * cs, const char * ct);
        if(strncmp(buff, "string=", 7)==0)   //poredi dva stringa za 7 karaktera (max broj karaktera) 
		{                                     //ako je to ispisano, kopiraj string                          
			strcpy(ppp, (buff+7));                 //iz kernela u ppp
			printk(KERN_WARNING "Succesfully wrote string\n");
		}
                                                                         //clear
		else if(strcmp(buff,"clear")==0)
		{
			printk(KERN_WARNING "Deleting string\n");
			for(j=0; j<100; j++)
				ppp[j]=0;              
		}
		
		else if(strcmp(buff, "append=Ovo je string koji se dodaje")==0)
		{
			strcpy(str1, buff);
			strcat(ppp, str1);
			printk(KERN_WARNING "Succesfully concatenated strings\n");
		}
		
		
		else if(strcmp(buff, "truncate=")==0)
		{
			ret=sscanf(buff, "%d,%d", &value,&position);
			if(ret==2)
			{
			  duzina=strlen(ppp);
			  duz=duzina-6;
			  if(position>duz)
			  {
				  ppp[strlen(ppp)-value]='\0';
				  printk(KERN_INFO "Succesfully deleted 5 end characters"); 
			  }
			  
			}
		}
		
		else if(strcmp(buff, "shrink")==0)
		{
			shrrink = strim(ppp);  
			s=0;
			while(ppp[s] != '\0')
			{
				ppp[s]=(*(shrrink+s));
				s++;
			}
			ppp[s]='\0';
			printk(KERN_WARNING "Succesfully deleted spaces\n");
		}
		
		else if(strcmp(buff, "remove=")==0)
		{
			//int valuee, positionn,dezi, duzinaa;
			printk(KERN_INFO "Which string would you like to remove?\n");
			//uporedim
			ret = copy_from_user(bufff, buffer, length);  //kopiramo iz niza buffer u niz buff (kernel prostor)
	        if(ret)
		    return -EFAULT;
	
	        bufff[length-1] = '\0'; 
			
			char *pch;
			while (pch = strstr(ppp, bufff));
		    {
				if(pch!=NULL)
				{
					*pch='\0';
                     strcat(ppp, pch+strlen(bufff));	
                     printk(KERN_INFO "Deleted"); 					
				}
			}
			
			printk(KERN_WARNING "Succesfully removed\n");
	/*		  if(pch != NULL)
			  {
		        memmove(pch+strlen(bufff), pch, strlen(buff));
				ret=sscanf(bufff, "%d,%d", &valuee,&positionn);
			    if(ret==2)
			  {
			    duzinaa=strlen(ppp);
			    duzi=duzinaa-(strlen(buff)+1);
			    if(position>duzi)
			    {
				  ppp[strlen(ppp)-valuee]='\0';
				  printk(KERN_INFO "Deleted"); 
			    }
			  
			   }
			  }
				
			
			}
			*/
	    } 
}
	else 
		 printk(KERN_WARNING "The string should not have more than 100 characters");


	return length;
   

}



static int __init ppp_init(void)
{
   int ret = 0;
   int i=0;
                            
	//Initialize array
   for (i=0; i<100; i++) ppp[i] = 0;
	

   ret = alloc_chrdev_region(&my_dev_id, 0, 1, "ppp");
   if (ret)
   {
      printk(KERN_ERR "failed to register char device\n");
      return ret;
   }
   printk(KERN_INFO "char device region allocated\n");

   my_class = class_create(THIS_MODULE, "ppp_class");
   if (my_class == NULL)
   {
      printk(KERN_ERR "failed to create class\n");
      goto fail_0;
   }
   printk(KERN_INFO "class created\n");
   
   my_device = device_create(my_class, NULL, my_dev_id, NULL, "ppp");
	if (my_device == NULL)
	{
      printk(KERN_ERR "failed to create device\n");
      goto fail_1;
	}
	printk(KERN_INFO "device created\n");

	my_cdev = cdev_alloc();	
	my_cdev->ops = &my_fops;
	my_cdev->owner = THIS_MODULE;
	ret = cdev_add(my_cdev, my_dev_id, 1);
	if (ret)
	{
      printk(KERN_ERR "failed to add cdev\n");
		goto fail_2;
	}
	printk(KERN_INFO "cdev added\n");
	printk(KERN_INFO "Hello world\n");

	return 0;

   fail_2:
      device_destroy(my_class, my_dev_id);
   fail_1:
      class_destroy(my_class);
   fail_0:
      unregister_chrdev_region(my_dev_id, 1);
   return -1;
}

static void __exit ppp_exit(void)
{
   cdev_del(my_cdev);
   device_destroy(my_class, my_dev_id);
   class_destroy(my_class);
   unregister_chrdev_region(my_dev_id,1);
   printk(KERN_INFO "Goodbye, cruel world\n");
   
}


module_init(ppp_init);
module_exit(ppp_exit);