#include <cstdio>
#include <cassert>
#include <cstring>

#include "unique_ptr.hpp"

struct dummy {
    int val;

    dummy() : val(42) {
        printf("Constructed! %d\n", val);
    }

    ~dummy() {
        printf("Destructed! %d\n", val);
    }
};


void do_something(dummy* d) {
    d->val++;
    printf("Do something called with pointer: %d\n", d->val);
}

void do_unique(my::unique_ptr<dummy> d) {
    d->val++;
    printf("Do something called with unique_ptr: %d\n", d->val);
}

void file_example();

int main(int argc, char const *argv[])
{
    my::unique_ptr<dummy> d(new dummy);
    assert(d.get() != NULL);

    // Cannot copy! Enforce unique ownership
    //my::unique_ptr<dummy> d2 = d; // does not compile
    //my::unique_ptr<dummy> d2(d); // does not compile
    
    // But can transfer ownership
    my::unique_ptr<dummy> d2 = d.move();
    assert(d2.get() != NULL);
    assert(d.get() == NULL);
    
    do_something(d2.get());
    assert(d2.get() != NULL);

    // Copying to a function does not compile
    //do_unique(d2); // does not compile

    // Transfer ownership to another function
    do_unique(d2.move());
    assert(d2.get() == NULL);
    printf("About to exit\n");

    // example showing customization of deleter
    file_example();

    return 0;
}


// =======================
// Demo of customizing unique_ptr for FILE handles
// C is used to demonstrate what C++ adds (automatic closing)

struct file_closer {
    void operator() (FILE* file) {
        printf("Closing file\n");
        fclose(file);
    }
};

typedef my::unique_ptr<FILE, file_closer> unique_file;

void file_example() {
    printf("Opening file\n");
    unique_file f(fopen("test.txt", "w"));

    char buf[5];

    // print numbers 0 to 10 to file
    for (int i = 0; i < 10; ++i)
    {
        snprintf(buf, 5, "%d\n", i);
        fwrite(buf, sizeof(char), strlen(buf), f.get());
    }
}
