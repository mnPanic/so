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

- read: Se devuelve una cadena de texto conteniendo un numero al azar entre - y
  el valor que el usuario escribio previamente, con un '\n'.

*/

#define MINOR_COUNT 1
#define MINOR 0
#define DEVICE_NAME "azar"

static struct cdev device;
static dev_t major;
static struct class *class;
static int rand_max;

static ssize_t azar_read(
		struct file *filp, 	// instancia del file
		char __user *data, 	// puntero al buff del usuario
		size_t s, 			// cantidad de bytes deseada
		loff_t *off			// offset, ignorado para char devices
	) {		
	// https://docs.huihoo.com/doxygen/linux/kernel/3.7/linux_2random_8h.html#a47ed2069dd5b87fbe6da11d34029b6d8
	// printk(KERN_ALERT "stored max: %d\n", rand_max);

	int rand = 0;
	get_random_bytes(&rand, sizeof(int));
	int trimmed_rand = rand % rand_max;
	if (trimmed_rand < 0) trimmed_rand *= -1;

	// printk(KERN_ALERT "rand: %d, mod_max: %d\n", rand, trimmed_rand);

	// https://www.kernel.org/doc/htmldocs/kernel-api/API-snprintf.html
	// do a dummy write to find out the size of the string to write
	ssize_t bufsz = snprintf(NULL, 0, "%d", trimmed_rand);
	char* buf = kmalloc(bufsz + 2, NULL);
	snprintf(buf, bufsz + 1, "%d", trimmed_rand);
	buf[bufsz + 1] = '\n'; // agrego newline

	// copy_to_user: https://www.fsl.cs.sunysb.edu/kernel-api/re256.html
	copy_to_user(data, buf, bufsz+2);

	kfree(buf);
	return bufsz+2;
}

static ssize_t azar_write(
		struct file *filp,			// instancia del file
		const char __user *data,	// puntero al buff del usuario
		size_t s, 					// cantidad de bytes deseada
		loff_t *off					// offset, ignorado para char devices
	) {
	// Copio los datos del usuario
	char cpy[s+1];

	// https://www.fsl.cs.sunysb.edu/kernel-api/re257.html
	copy_from_user(&cpy[0], data, s);
	cpy[s] = '\0'; // le agrego null-terminator

	printk(KERN_ALERT "got: %s\n", cpy);
	
	// https://www.kernel.org/doc/htmldocs/kernel-api/API-kstrtoint.html
	// int kstrtoint(const char* s, unsigned int base, int* res)
	kstrtoint(cpy, 10, &rand_max);

	return s;
}

static struct file_operations operations = {
	.owner = THIS_MODULE,
	.read  = azar_read,
	.write = azar_write,
};

static int __init azar_init(void) {
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

	// por defecto tomo rand_max como 0
	rand_max = 0;

	printk(KERN_ALERT "cargando azar\n");
	return 0;
}

static void __exit azar_exit(void) {
	// Deshacemos los pasos realizados en init()
	unregister_chrdev_region(major, MINOR_COUNT);
	cdev_del(&device);

	// Destruccion de los nodos en el fs
	device_destroy(class, major);
	class_destroy(class);

	printk(KERN_ALERT "sacando azar\n");
}

module_init(azar_init);
module_exit(azar_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una suerte de 'Hola, mundo'");
