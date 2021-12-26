/*
    Author: Rohan Deshpande
    Date: November 25, 2021

    Combinational-Circuit-Language
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct TableNode
{
    char *str;
    bool val;
    struct TableNode *next;
};
typedef struct TableNode TabNode;

void free_list(struct TableNode *main_list)
{
    struct TableNode *temp;

    while (main_list != NULL)
    {
        temp = main_list;
        main_list = main_list->next;
        free(temp->str);
        free(temp);
    }
}
void print_list(TabNode *main_list)
{
    TabNode *pointer_main_list = main_list;
    if (pointer_main_list == NULL)
    {
        printf("List is empty!");
    }
    while (pointer_main_list != NULL)
    {
        int binary_val = pointer_main_list->val ? 1 : 0;
        printf("Variable: %s, Binary: %d", pointer_main_list->str, binary_val);
        pointer_main_list = pointer_main_list->next;
    }
    printf("\n");
}
void print_table(TabNode **table, int size)
{
    TabNode **table_temp = table;
    for (int i = 0; i < size; i++)
    {
        if (table_temp[i] != NULL)
        {
            printf("Hash: %d ", i);
            print_list(table_temp[i]);
        }
    }
}
void free_char_array(char **free_char_array, int row)
{
    for (int i = 0; i < row; i++)
    {
        free(free_char_array[i]);
    }
    free(free_char_array);
}

TabNode **create_table(int max_size)
{
    /*
        Create a table.
        KEY :: int : The sum of the ascii value of the string
        VALUE :: TableNode :Linked list of type TableNode
    */
    TabNode **table = (TabNode **)malloc(max_size * sizeof(TabNode *));
    for (int i = 0; i < max_size; i++)
    {
        table[i] = NULL;
    }
    return table;
}
void free_table(TabNode **table, int max_size)
{
    for (int i = 0; i < max_size; i++)
    {
        free_list(table[i]);
    }
    free(table);
}
int hash_value(char *str)
{
    int add_ascii = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        add_ascii += str[i];
    }
    return add_ascii;
}
void add_to_table(TabNode **table, char *add_str, bool binary_value)
{
    /*
        For a unique ascii hash key U_K, create a new linked-list at that U_K and put the new string as a node in that new linked list.
        For a non-unique ascii hash key U_K, Update the exisiing linked-list at that place by appending the node to the end of linked list.
    */
    int hash = hash_value(add_str);

    if (table[hash] == NULL)
    {
        table[hash] = malloc(sizeof(TabNode));
        table[hash]->str = add_str;
        table[hash]->val = binary_value;
        table[hash]->next = NULL;
    }
    else
    {
        TabNode *temp_table = table[hash];
        while (temp_table->next != NULL)
        {
            temp_table = temp_table->next;
        }
        temp_table->next = malloc(sizeof(TabNode));
        temp_table->next->str = add_str;
        temp_table->next->val = binary_value;
        temp_table->next->next = NULL;
    }
}
void update_table_by_string(TabNode **table, char *index_str, bool binary_value)
{
    /*
        This method will update node to the hash-table based on the input strings hash value.
        Since add_to_table is called before update, we can assume that all variables in the file exist in the table.

        Hence then we could can compare a variable at its hash by going through the linked list, and once we find
        the correct string then we can just update that node's binary_val value.
    */
    int hash = hash_value(index_str);

    bool found_string = false;

    TabNode *temp = table[hash];
    while (!found_string && temp != NULL)
    {
        if (strcmp(index_str, temp->str) == 0)
        {
            found_string = true;
            temp->val = binary_value;
        }
        temp = temp->next;
    }
}
int index_table_by_string(TabNode **table, char *index_str)
{
    /*
        This method will index the table for the given string and return the binary value at that located node.
        If node is not found then -1 is returned.
    */
    int hash = hash_value(index_str);
    if (table[hash] == NULL)
    {
        if (strcmp(index_str, "1") == 0)
        {
            return 1;
        }
        if (strcmp(index_str, "0") == 0)
        {
            return 0;
        }
        return -1;
    }
    else
    {
        TabNode *temp = table[hash];
        while (temp != NULL)
        {
            if (strcmp(index_str, temp->str) == 0)
            {
                return temp->val ? 1 : 0;
            }
            temp = temp->next;
        }
        return -1;
    }
}
int read_variable(char *read_var, FILE *file_pointer)
{
    int read_status = fscanf(file_pointer, "%16s", read_var);
    return read_status;
}
int read_number(int *read_var, FILE *file_pointer)
{
    int read_status = fscanf(file_pointer, "%16d", read_var);
    return read_status;
}
bool *generate_boolean_permutation(int num_inputs, int row)
{
    bool *binary_rep = calloc(num_inputs, sizeof(int));
    int i = 0;
    while (row > 0)
    {
        binary_rep[i] = ((row % 2) == 1);
        row /= 2;
        i++;
    }
    for (int i = 0; i < num_inputs / 2; i++)
    {
        bool temp = binary_rep[i];
        binary_rep[i] = binary_rep[num_inputs - 1 - i];
        binary_rep[num_inputs - 1 - i] = temp;
    }
    return binary_rep;
}
int main(int argc, char **argv)
{
    int max_size = 16 * 255; // since each variable has atmost 16 characters, and the max ascii is 255 then the max hash value can be 255+255+255...+255 = 255 * 16
    TabNode **table = create_table(max_size);

    // handle file i/o here
    FILE *file_pointer_circuit = fopen(argv[1], "r");
    if (file_pointer_circuit == NULL)
    {
        printf("%s\n", "Couldn't read the file at the given location!");
        return EXIT_FAILURE;
    }

    char input_string[17]; // This will read "INPUT"
    int num_inputs = 0;
    int num_outputs = 0;

    read_variable((char *)&input_string, file_pointer_circuit); // A wrapper function that just runs fscanf in the backend

    if (strcmp(input_string, "INPUT") == 0)
    {
        read_number(&num_inputs, file_pointer_circuit);
    }

    char **input_strs = (char **)malloc(sizeof(char *) * num_inputs);
    ;
    char **output_strs;
    char *output_variables_or_and = NULL;
    char *output_variables_not_pass = NULL;
    char *output_variables_decoder = NULL;
    char *output_variables_multiplexer = NULL;

    int compute_possible_combinations = 1 << num_inputs; // This is the total number of rows that is going to be printed.

    for (int row = 0; row < compute_possible_combinations; row++)
    {
        bool *binary_perm = generate_boolean_permutation(num_inputs, row);
        // For the first row, fill the table with the input values and it's respective n-bit values and for other rows keep updating the n-bit values.
        for (int j = 0; j < num_inputs; j++)
        {
            if (row == 0)
            {
                input_strs[j] = (char *)malloc(sizeof(char) * 17);
                read_variable(input_strs[j], file_pointer_circuit);
                add_to_table(table, input_strs[j], binary_perm[j]);
            }
            else
            {
                update_table_by_string(table, input_strs[j], binary_perm[j]);
            }
        }

        free(binary_perm);

        char output_string[17];                                      // This will read "OUTPUT"
        read_variable((char *)&output_string, file_pointer_circuit); // A wrapper function that just runs fscanf in the backend
        if (row == 0 && strcmp(output_string, "OUTPUT") == 0)
        {
            read_variable(output_string, file_pointer_circuit);
            num_outputs = atoi(output_string);                           // reads the number of outputs
            output_strs = (char **)malloc(sizeof(char *) * num_outputs); // TODO: free this shit... this is a string array for storing multiple strings
            for (int i = 0; i < num_outputs; i++)
            {
                output_strs[i] = (char *)malloc(sizeof(char) * 17);
                read_variable(output_strs[i], file_pointer_circuit);
                add_to_table(table, output_strs[i], false); // init output with 0
            }
        }
        while (true)
        {
            char circuit_string[17];                                                      // This will read circuit instructions
            int check_var = read_variable((char *)&circuit_string, file_pointer_circuit); // A wrapper function that just runs fscanf in the backend

            // EOF Reached!
            if (check_var != 1)
            {
                break;
            }
            if (strcmp(circuit_string, "OR") == 0 || strcmp(circuit_string, "NAND") == 0 || strcmp(circuit_string, "AND") == 0 || strcmp(circuit_string, "NOR") == 0 || strcmp(circuit_string, "XOR") == 0)
            {
                char **two_input_circuits = (char **)malloc(sizeof(char *) * 2);
                int bitwise_op = 0;
                for (int i = 0; i < 2; i++)
                {
                    two_input_circuits[i] = (char *)malloc(sizeof(char) * 17);
                    read_variable(two_input_circuits[i], file_pointer_circuit);
                    int binary_value = index_table_by_string(table, two_input_circuits[i]);
                    if (binary_value == -1)
                    {
                        // print_table(table, max_size);
                        printf("OR/AND Couldn't index a binary value %s for the given input variable. Check if input variable even exists in the table?", two_input_circuits[i]);
                        break;
                    }
                    int binary_value_int = binary_value == true ? 1 : 0;
                    if (strcmp(circuit_string, "OR") == 0)
                    {
                        bitwise_op = i == 0 ? binary_value_int : binary_value_int | bitwise_op;
                    }
                    else if (strcmp(circuit_string, "NAND") == 0)
                    {
                        bitwise_op = i == 0 ? binary_value_int : !(binary_value_int & bitwise_op);
                    }
                    else if (strcmp(circuit_string, "AND") == 0)
                    {
                        bitwise_op = i == 0 ? binary_value_int : binary_value_int & bitwise_op;
                    }
                    else if (strcmp(circuit_string, "NOR") == 0)
                    {
                        bitwise_op = i == 0 ? binary_value_int : !(binary_value_int | bitwise_op);
                    }
                    else if (strcmp(circuit_string, "XOR") == 0)
                    {
                        bitwise_op = i == 0 ? binary_value_int : binary_value_int ^ bitwise_op;
                    }
                }
                if (row == 0)
                {
                    output_variables_or_and = (char *)malloc(sizeof(char) * 17);
                    read_variable(output_variables_or_and, file_pointer_circuit); // reads output variable here
                    add_to_table(table, output_variables_or_and, bitwise_op);     // updates the output variable's binary value with the correct bitwise_op
                    update_table_by_string(table, output_variables_or_and, bitwise_op);
                }
                else
                {
                    read_variable(output_variables_or_and, file_pointer_circuit);
                    update_table_by_string(table, output_variables_or_and, bitwise_op);
                }
                free_char_array(two_input_circuits, 2); // free the input variable array of the circuit instruction.
            }
            else if (strcmp(circuit_string, "NOT") == 0 || strcmp(circuit_string, "PASS") == 0)
            {
                int bitwise_op = 0;
                char *one_input_circuits = (char *)malloc(sizeof(char) * 17); // free
                read_variable(one_input_circuits, file_pointer_circuit);

                int binary_value = index_table_by_string(table, one_input_circuits);
                if (binary_value == -1)
                {
                    printf("NOT/PASS Couldn't index a binary value %s for the given input variable. Check if input variable even exists in the table?", one_input_circuits);
                    break;
                }

                if (strcmp(circuit_string, "NOT") == 0)
                {
                    bitwise_op = !binary_value;
                }
                else if (strcmp(circuit_string, "PASS") == 0)
                {
                    bitwise_op = binary_value;
                }
                if (row == 0)
                {
                    output_variables_not_pass = (char *)malloc(sizeof(char) * 17);
                    read_variable(output_variables_not_pass, file_pointer_circuit); // reads output variable here
                    add_to_table(table, output_variables_not_pass, bitwise_op);     // updates the output variable's binary value with the correct bitwise_op
                    update_table_by_string(table, output_variables_not_pass, bitwise_op);
                }
                else
                {
                    read_variable(output_variables_not_pass, file_pointer_circuit);
                    update_table_by_string(table, output_variables_not_pass, bitwise_op);
                }
                free(one_input_circuits);
            }
            else if (strcmp(circuit_string, "DECODER") == 0)
            {
                char string_decoder_input_length[17];
                read_variable(string_decoder_input_length, file_pointer_circuit);

                int decoder_input_size = atoi(string_decoder_input_length);
                char **n_input_circuits = (char **)malloc(sizeof(char *) * decoder_input_size);
                int sum_binary = 0;
                for (int i = decoder_input_size - 1; i >= 0; i--)
                {
                    n_input_circuits[i] = (char *)malloc(sizeof(char) * 17);

                    read_variable(n_input_circuits[i], file_pointer_circuit);
                    int binary_value = index_table_by_string(table, n_input_circuits[i]);
                    sum_binary += ((1 << i) * binary_value);

                    free(n_input_circuits[i]);
                }

                int decoder_output_size = 1 << decoder_input_size;

                for (int j = 0; j < decoder_output_size; j++)
                {
                    bool bitwise_op = j == sum_binary ? true : false;
                    if (row == 0)
                    {
                        output_variables_decoder = (char *)malloc(sizeof(char) * 17);
                        read_variable(output_variables_decoder, file_pointer_circuit);
                        add_to_table(table, output_variables_decoder, bitwise_op);
                        update_table_by_string(table, output_variables_decoder, bitwise_op);
                    }
                    else
                    {
                        read_variable(output_variables_decoder, file_pointer_circuit);
                        update_table_by_string(table, output_variables_decoder, bitwise_op);
                    }
                }
                free(n_input_circuits);
            }
            else if (strcmp(circuit_string, "MULTIPLEXER") == 0)
            {
                char string_multiplexer_selector_length[17];
                read_variable(string_multiplexer_selector_length, file_pointer_circuit);

                int selector_input_size = atoi(string_multiplexer_selector_length);
                int multiplexer_input_size = 1 << selector_input_size;

                char **n_input_circuits = (char **)malloc(sizeof(char *) * multiplexer_input_size);
                char **n_selector_circuits = (char **)malloc(sizeof(char *) * selector_input_size);

                for (int i = 0; i < multiplexer_input_size; i++)
                {
                    n_input_circuits[i] = (char *)malloc(sizeof(char) * 17);
                    read_variable(n_input_circuits[i], file_pointer_circuit);
                }
                int base_10_selector_value = 0;
                for (int j = selector_input_size - 1; j >= 0; j--)
                {
                    n_selector_circuits[j] = (char *)malloc(sizeof(char) * 17);
                    read_variable(n_selector_circuits[j], file_pointer_circuit);

                    int binary_value = index_table_by_string(table, n_selector_circuits[j]);
                    base_10_selector_value += ((1 << j) * binary_value);
                }

                int binary_value = index_table_by_string(table, n_input_circuits[base_10_selector_value]);
                bool binary_rep = binary_value == 1 ? true : false;
                if (row == 0)
                {
                    output_variables_multiplexer = (char *)malloc(sizeof(char) * 17);
                    read_variable(output_variables_multiplexer, file_pointer_circuit);
                    add_to_table(table, output_variables_multiplexer, binary_rep);
                    update_table_by_string(table, output_variables_multiplexer, binary_rep);
                }
                else
                {
                    read_variable(output_variables_multiplexer, file_pointer_circuit);
                    update_table_by_string(table, output_variables_multiplexer, binary_rep);
                }

                free_char_array(n_input_circuits, multiplexer_input_size);
                free_char_array(n_selector_circuits, selector_input_size);
            }
        }
        for (int n_i = 0; n_i < num_inputs; n_i++)
        {
            int input_binary_value = index_table_by_string(table, input_strs[n_i]);
            printf("%d ", input_binary_value);
        }
        printf("|");
        for (int n_o = 0; n_o < num_outputs; n_o++)
        {
            int output_binary_value = index_table_by_string(table, output_strs[n_o]);
            printf(" %d", output_binary_value);
        }
        printf("\n");
        if (fseek(file_pointer_circuit, 0L, SEEK_SET) != 0)
        {
            printf("%s\n", "Couldn't read the file at the given location!");
            return EXIT_FAILURE;
        }
    }
    free_table(table, max_size);

    free(input_strs);
    free(output_strs);
}
