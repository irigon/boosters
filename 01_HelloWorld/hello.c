//Filename: hello.c
#include <linux/module.h>
#include <linux/init.h>

// this function is called when our module is loaded
static int __init setup(void) {
    printk(KERN_INFO "Hello, world!\n");
    return 0;
}

// this function is called when the kernel is unloaded
static void __exit teardown(void) {
    printk(KERN_INFO "Bye!\n");
}

// module_init defines the setup function; 
module_init(setup);

// module_exit defines the function to be called on the module unload.
module_exit(teardown);
