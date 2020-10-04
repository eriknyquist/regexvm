#ifndef RXVM_STACK_H
#define RXVM_STACK_H

ir_stack_t *create_stack (void);
stackitem_t *stack_add_head (ir_stack_t *stack, void *data);
stackitem_t *stack_add_tail (ir_stack_t *stack, void *data);
void stack_point_new_head (ir_stack_t *stack, stackitem_t *new);
void stack_free_head (ir_stack_t *stack);
void stack_cat (ir_stack_t *stack1, ir_stack_t *stack2);
void stack_free (ir_stack_t *stack, void (*cleanup) (void *));

#endif
