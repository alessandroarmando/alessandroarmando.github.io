*Key Question:* What happens is the main terminates before the newly created thread?

The main thread (which runs the main function) creates the thread parameter structures as local variables, and then passes pointers to these structures to the threads it creates.
If `main` finishes executing before the child thread is done, the memory containing the thread parameter structures will be deallocated while the other thread is still accessing it!

One solution is to force main to wait until the thread is done.

What we need is a function that waits for a thread to finish.

`pthread_join` takes two arguments:
* the thread ID of the thread to wait for
* a pointer to a `void *` variable that will receive the finished thread’s return value.
If you don’t care about the thread return value, pass NULL as the second argument.

### Example (correct)
The correct version of the previous program is available [here](code/thread_create_ok.c).