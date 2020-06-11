#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cdev.h> // char device
#include <linux/fs.h>	// file_operations
#include <linux/device.h>

static ssize_t nulo_read(
		struct file *filp, 	// instancia del file
		char __user *data, 	// puntero al buff del usuario
		size_t s, 			// cantidad de bytes deseada
		loff_t *off) {		// offset, ignorado para char devices
	printk(KERN_ALERT "leyendo en nulo\n");

	// no hay nada pa leer aca
	return 0;
}

static ssize_t nulo_write(
		struct file *filp,			// instancia del file
		const char __user *data,	// puntero al buff del usuario
		size_t s, 					// cantidad de bytes deseada
		loff_t *off) {				// offset, ignorado para char devices
	printk(KERN_ALERT "escribiendo en nulo\n");

	// "escribi" s
	return s;
}

static struct file_operations operations = {
	.owner = THIS_MODULE,
	.read  = nulo_read,
	.write = nulo_write,
};

static struct cdev device;
static dev_t major;
static struct class *class;

#define MINOR_COUNT 1
#define MINOR 0
#define DEVICE_NAME "nulo"

static int __init nulo_init(void) {
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

	printk(KERN_ALERT "cargando nulo\n");
	return 0;
}

static void __exit nulo_exit(void) {
	// Deshacemos los pasos realizados en init()
	unregister_chrdev_region(major, MINOR_COUNT);
	cdev_del(&device);

	// Destruccion de los nodos en el fs
	device_destroy(class, major);
	class_destroy(class);

	printk(KERN_ALERT "sacando nulo\n");
}

module_init(nulo_init);
module_exit(nulo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("La banda de SO");
MODULE_DESCRIPTION("Una suerte de 'Hola, mundo'");
