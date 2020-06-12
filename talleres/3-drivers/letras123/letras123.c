#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h> 	// char device
#include <linux/fs.h>		// file_operations
#include <linux/device.h>	// class for fs
#include <linux/uaccess.h> 	// copy to & from user
#include <linux/kernel.h>	// kstrtoint
#include <linux/random.h>	// get_random_bytes
#include <linux/slab.h>		// kmalloc / free

/* Spec del driver

- write: Se interpreta el contenido del buffer como un string al que se le
  agrega un null terminator ('\0'), y este representa un numero entero que es
  almacenado.

- read: Se devuelve una cadena de texto conteniendo un numero al letras123 entre - y
  el valor que el usuario escribio previamente, con un '\n'.

*/

#define MINOR_COUNT 1
#define MINOR 0
#define DEVICE_NAME "letras123"

static struct cdev device;
static dev_t major;
static struct class *class;


typedef struct slot_s {
	char c;
	bool taken;
} slot_t;

static slot_t slots[3];
static spinlock_t slots_lock;

static ssize_t letras123_open(struct inode* node, struct file* fp) {
	printk(KERN_ALERT "open\n");
	spin_lock(&slots_lock);
	int i = 0;
	while(i < 3) {
		if(slots[i].taken == 0) {
			slots[i].taken = 1;
			fp->private_data = i;
			printk(KERN_ALERT "took %d\n", i);
			break;
		}
		i++;
	}
	spin_unlock(&slots_lock);

	return 0;
}

static ssize_t letras123_release(struct inode* node, struct file* fp) {
	spin_lock(&slots_lock);

	int idx = (int)fp->private_data;
	printk(KERN_ALERT "close %d\n", idx);
	slots[idx].taken = 0;
	slots[idx].c = 0;

	spin_unlock(&slots_lock);
	return 0;
}


static ssize_t letras123_read(
		struct file *fp, 	// instancia del file
		char __user *data, 	// puntero al buff del usuario
		size_t s, 			// cantidad de bytes deseada
		loff_t *off			// offset, ignorado para char devices
	) {
	spin_lock(&slots_lock);

	int idx = (int)fp->private_data;
	if(slots[idx].c == 0) {
		spin_unlock(&slots_lock);
		return -EPERM;
	}

	char buf[s];
	int i = 0;
	while (i < s) {
		buf[i] = slots[idx].c;
		i++;
	}

	copy_to_user(data, buf, s);

	spin_unlock(&slots_lock);
	return s;
}

static ssize_t letras123_write(
		struct file *fp,			// instancia del file
		const char __user *data,	// puntero al buff del usuario
		size_t s, 					// cantidad de bytes deseada
		loff_t *off					// offset, ignorado para char devices
	) {
	spin_lock(&slots_lock);

	// Leo el 1er caracter
	char c;
	copy_from_user(&c, data, 1);
	printk(KERN_ALERT "write %c\n", c);

	// Si no tenia nada, lo escribo
	int idx = (int)fp->private_data;
	if (slots[idx].c == 0) {
		slots[idx].c = c;
	}

	spin_unlock(&slots_lock);
	return s;
}

static struct file_operations operations = {
	.owner   = THIS_MODULE,
	.read    = letras123_read,
	.write	 = letras123_write,
	.open    = letras123_open,
	.release = letras123_release,
};

static int __init letras123_init(void) {
	// TODO: error handling

	// Inicializamos como dispostivo
	cdev_init(&device, &operations);

	// Rerservamos major y minor
	//  - major: representa al driver
	//  - minor: representa a la instancia del driver
	alloc_chrdev_region(&major, MINOR, MINOR_COUNT, DEVICE_NAME);

	// Los asociamos al dispositivo creado
	cdev_add(&device, major, MINOR_COUNT);

	// Solicitamos la creacion del nodo en el file system
	class = class_create(THIS_MODULE, DEVICE_NAME);
	device_create(class, NULL, major, NULL, DEVICE_NAME);

	printk(KERN_ALERT "cargando letras123\n");

	// inicializo los slots en 0
	int i = 0;
	while(i < 3) {
		slots[i].taken = 0;
		slots[i].c = NULL;
		i++;
	}

	spin_lock_init(&slots_lock);

	return 0;
}

static void __exit letras123_exit(void) {
	// Deshacemos los pasos realizados en init()
	unregister_chrdev_region(major, MINOR_COUNT);
	cdev_del(&device);

	// Destruccion de los nodos en el fs
	device_destroy(class, major);
	class_destroy(class);

	printk(KERN_ALERT "sacando letras123\n");
}

module_init(letras123_init);
module_exit(letras123_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una suerte de 'Hola, mundo'");
