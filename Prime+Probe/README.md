# Prime+Probe

## Compilation instructions
`bash
gcc receiver.c util.c -o recv -lrt
gcc sender.c util.c -o send -lrt
`
## Execution
You can use taskset to specify the core to run the program on.
The command line arguments are the set number, the slice number and the time quantum or delay in microseconds.

The sender and the receiver will access its /proc/pagemap file and so it needs superuser privileges.

Before executing, allocate around 20 hugepages by executing the following as root
`bash
sudo su
echo 20 > /sys/kernel/mm/hugepages/hugepage-2048kB/nr_hugepages
`

To execute run the following 2 commands in two different terminals.
The sender generates a random bit sequence and sends it to the receiver. Both of them will write the sequence to a file which can be used to check for correctness.

`bash
taskset -c 1 sudo ./recv 2033 2 1000
taskset -c 2 sudo ./send 2033 2 1000
`

One way to check for correctness is to find the number of bits that differ. This can be done with the following command.
`bash
diff -y --suppress-common-lines 10000-send 10000-recv | wc -l
`

