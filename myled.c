/*
 *  *Copyright (c) 2020 Koki Shu.
 *  This program is a GNUGeneral Public License and is a free copyleft license for software and other types of works.
 *   */
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
//#include <linux/delay.h> //msleepのため

MODULE_AUTHOR("Ryuichi Ueda & Koki Shu");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL; //あくまでクラスという構造体
static volatile u32 *gpio_base = NULL; //アドレスをマッピングするための配列をグローバルで定義（動かすなよ）

static int gpio[5] = {21, 12, 16, 9, 25};

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	char c;
	int i;
	if(copy_from_user(&c, buf, sizeof(char)))
		return -EFAULT;

	//printk(KERN_INFO "receive %c\n", c);
	
	if(c == '0'){
		for(i=0; i<5; i++){
		gpio_base[10] = 1 << gpio[i];
		}
	}
	else if(c == 'a')
		gpio_base[7] = 1 << gpio[0];
	else if(c == 'b')
		gpio_base[7] = 1 << gpio[1];
	else if(c == 'c')
		gpio_base[7] = 1 << gpio[2];
	else if(c == 'd')
		gpio_base[7] = 1 << gpio[3];
	else if(c == 'e')
		gpio_base[7] = 1 << gpio[4];
/*	else if(c == '1'){
		for(i=0; i<5; i++){
			gpio[7] = 1 << gpio[i];
			msleep(1000);
		}
	}
*/		

	return 1;//１バイト読んだよ（ダミー）　０にすると止まらない　システムが止まり再起動が必要
}
static ssize_t sushi_read(struct file* filp, char* buf, size_t count, loff_t* pos)
{
	int size = 0;
	char sushi[] = {'s', 'u', 's', 'h', 'i', 0x0A};
	if(copy_to_user(buf+size, (const char *)sushi, sizeof(sushi))){
		printk( KERN_INFO "sushi : copy_to_user failed\n");
		return -EFAULT;
	}
	size += sizeof(sushi);
	return size;

}

static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write,
	.read = sushi_read
};

static int __init init_mod(void) //カーネルモジュールの初期化（使ったら自分を消し去る）
{
	int retval, i;
	retval = alloc_chrdev_region(&dev, 0, 1,"myled");//デバイス番号の取得　デバイス名：myled
	if(retval < 0){
		printk(KERN_ERR "alloc_chrdev_region failed.\n"); //ログを吐く　stdio.hはカーネルの中で動かない
		return retval;
	}
	printk(KERN_INFO "%s is loaded. major:%d\n", __FILE__, MAJOR(dev));

	cdev_init(&cdv, &led_fops);
	retval = cdev_add(&cdv, dev, 1);
	if(retval < 0){
		printk(KERN_ERR "cdev_add failed. major:%d, minor:%d",MAJOR(dev),MINOR(dev));
		return retval;
	}

	cls = class_create(THIS_MODULE, "myled");
	if(IS_ERR(cls)){
		printk(KERN_ERR "class_create failed.");
		return PTR_ERR(cls);
	}
	device_create(cls, NULL, dev, NULL,"myled%d", MINOR(dev));

	gpio_base = ioremap_nocache(0xfe200000, 0xA0);//Pi4ゆえ（Pi2はfeが３fに)

	for(i = 0; i < 5; i++){
	const u32 led = gpio[i];
	const u32 index = led/10;
	const u32 shift = (led%10)*3;
	const u32 mask = ~(0x7 << shift);// ・・・111000111111111111111
	gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);
	//001がoutputに相当
	}
	return 0;
}

static void __exit cleanup_mod(void) //後始末
{
	cdev_del(&cdv); //順番の逆転に注意
        device_destroy(cls, dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded. major:%d\n", __FILE__, MAJOR(dev));
}

module_init(init_mod); //OSから関数を読みだしてもらうためにマクロに登録
module_exit(cleanup_mod);
