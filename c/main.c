/*
    COSC 325 C Imperative Paradigm Lab
    
    Part 1
    
    Task 1.1 Prediction:
    I think x will be 20 because:
    x = 2
    x = 2 + 3 = 5
    x = 5 * 4 = 20
    
    Reflection 1.1:
    The value of x changed over time with each assignment statement.
    The intermediate values lived in the memory location assigned to 
    variable x, and each assignment overwrote the previous value.
    
    Task 1.2
    After swapping multiply and add:
    x = 2 * 4 = 8
    x = 8 + 3 = 11
    
    Reflection 1.2:
    Yes the meaning changed because imperative languages execute 
    statements in order, so changing the sequence changes the result.
    
    Part 2
    
    Reflection 2.1:
    Control happens in the for loop line (line 40).
    State changes in the line where s += i (linen 41).
    
    Reflection 2.2:
    Python and Java versions hide the loop structure and the state variable.
    They abstract away the explicit iteration and accumulation that we had 
    to write manually in C.
    
    Part 3
    
    Reflection 3.1:
    C requires explicit type casting to convert int to double to avoid 
    integer division. Java/Python handle this conversion automatically.
    
    Part 4
    
    Reflection 4.1:
    Variable a didn't change because C uses pass by value, which means
    the function gets a copy of the value, not the original variable.
    
    Reflection 4.2:
    A pointer gives you the memory address of a variable so you can 
    directly modify the original variable instead of just a copy.
*/

#include <stdio.h>

// Pt2 - sum_to function
int sum_to(int n) {
    int s = 0;
    for (int i = 1; i <= n; i++) {
        s += i;
    }
    return s;
}

// Pt3 - avg_1_to_n function
double avg_1_to_n(int n) {
    int total = sum_to(n);
    double avg = (double)total / n;
    return avg;
}

// Pt4 - broken increment (doesn't work)
void increment_bad(int x) {
    x++;
}

// Pt4 - fixed increment with pointer
void increment(int *x) {
    (*x)++;
}

int main() {
    
    // Pt1 - Testing variable assignments
    printf("Pt1\n");
    int x = 2;
    x = x + 3;
    x = x * 4;
    printf("x=%d\n", x);
    
    printf("\n");
    
    // Pt2 - Testing sum_to
    printf("Pt2\n");
    printf("sum_to(5) = %d\n", sum_to(5));
    printf("sum_to(1) = %d\n", sum_to(1));
    
    printf("\n");
    
    // Pt3 - Testing avg_1_to_n
    printf("Pt3\n");
    printf("avg_1_to_n(5) = %.1f\n", avg_1_to_n(5));
    
    printf("\n");
    

    printf("Pt4\n");
    int a = 5;
    increment_bad(a);
    printf("After increment_bad: a=%d\n", a);
    
    int b = 5;
    increment(&b);
    printf("After increment: b=%d\n", b);
    
    return 0;
}
