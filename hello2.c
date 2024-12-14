/* hello2_modified.c */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include "hello1.h"

MODULE_AUTHOR("Nazar Bychkar <nazar@bychkar.com>");
MODULE_DESCRIPTION("Hello2 module");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int hello_count = 1;
module_param(hello_count, uint, 0444);
MODULE_PARM_DESC(hello_count, "Specifies the number of 'Hello, world!' messages to print");

static int __init module_initialize(void)
{
    if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10)) {
        pr_warn("Caution: hello_count = %d\n", hello_count);
    }

    if (hello_count > 10) {
        pr_err("Invalid value: hello_count = %d\n", hello_count);
        return -EINVAL;
    }

    print_hello_messages(hello_count);
    return 0;
}

static void __exit module_cleanup(void)
{
    pr_info("Hello2 module has been removed\n");
}

module_init(module_initialize);
module_exit(module_cleanup);

