#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

typedef enum
{
    AND,
    OR,
    NAND,
    NOR,
    XOR,
    NOT,
    PASS,
    DECODER,
    MULTIPLEXER
} kind_t;

struct gate
{
    kind_t kind;
    int size;    // indicates size of DECODER and MULTIPLEXER
    int *params; // length determined by kind and size;
    // includes inputs and outputs, indicated by variable numbers
};

struct TableNode
{
    char *str;
    bool val;
    struct TableNode *next;
};
typedef struct TableNode TabNode;

void print_list(TabNode *main_list)
{
    TabNode *pointer_main_list = main_list;
    while (pointer_main_list != NULL)
    {
        int binary_val = pointer_main_list->val ? 1 : 0;
        printf("Variable: %s, Binary: %d", pointer_main_list->str, binary_val);
        pointer_main_list = pointer_main_list->next;
    }
    free(pointer_main_list);
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
        Create a table from a 1-d array.
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
        For a non-unique ascii hash key U_K, Update the exisiing linked-list at that place and append the node to the end of linked list.

        TODO: Maybe handle adding duplicate variables?
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
    int hash = hash_value(index_str);
    // This is for temporary variables with a unique hash
    if (table[hash] == NULL)
    {
        add_to_table(table, index_str, binary_value);
    }
    else
    {
        bool found_string = false;

        TabNode *temp = table[hash];
        TabNode *prev = table[hash];
        while (temp != NULL)
        {
            if (strcmp(index_str, temp->str) == 0)
            {
                found_string = true;
                temp->val = binary_value;
            }
            prev = temp;
            temp = temp->next;
        }
        /*
            Suppose a new variable has the same hash of a exisitng variable in the table, then a collison occurs.
            Hence, we need to add the new variable to the of the chain.
        */
        if (!found_string)
        {
            prev->next = malloc(sizeof(TabNode));
            prev->next->str = index_str;
            prev->next->val = binary_value;
            prev->next->next = NULL;
            found_string = true;
        }
    }
}
int index_table_by_string(TabNode **table, char *index_str)
{
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
        return -1; // Returns the binary value at the given variable in the table.
    }
}

int read_variable(char *read_var, FILE *file_pointer)
{
    int read_status = fscanf(file_pointer, "%16s", read_var);
    return read_status;
}
bool check_current_string_is_part_of_outputs(char **outputs, int num_outputs, char *output)
{
    for (int i = 0; i < num_outputs; i++)
    {
        if (strcmp(outputs[i], output) != 0)
        {
            return false;
        }
    }
    return true;
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
    int max_size = 16 * 255;                  // since each variable has atmost 16 characters, and the max ascii is 255 then the max hash value can be 255+255+255...+255 = 255 * 16
    TabNode **table = create_table(max_size); // TODO: free this table at the end

    // handle file i/o here
    FILE *file_pointer_circuit = fopen(argv[1], "r");
    if (file_pointer_circuit == NULL)
    {
        printf("%s\n", "Couldn't read the file at the given location!");
        return EXIT_FAILURE;
    }

    char input_string[17]; // This will read "INPUT"
    int num_inputs = 0;
    read_variable((char *)&input_string, file_pointer_circuit); // A wrapper function that just runs fscanf in the backend

    if (strcmp(input_string, "INPUT") == 0)
    {
        read_variable(input_string, file_pointer_circuit);
        num_inputs = atoi(input_string); // reads the number of inputs
    }
    char **input_strs = (char **)malloc(sizeof(char *) * num_inputs); // TODO: free this shit... this is a string array for storing multiple strings
    char **output_strs;
    int num_outputs = 0;

    int compute_possible_combinations = ((int)pow(2, num_inputs));
    for (int row = 0; row < compute_possible_combinations; row++)
    {
        bool *binary_perm = generate_boolean_permutation(num_inputs, row); // maybe free?

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
        char output_string[17];                                      // This will read "OUTPUT"
        read_variable((char *)&output_string, file_pointer_circuit); // A wrapper function that just runs fscanf in the backend
        if (strcmp(output_string, "OUTPUT") == 0)
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
                int biwise_op = 0;
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
                        biwise_op = i == 0 ? binary_value_int : binary_value_int | biwise_op;
                    }
                    else if (strcmp(circuit_string, "NAND") == 0)
                    {
                        biwise_op = i == 0 ? binary_value_int : !(binary_value_int & biwise_op);
                    }
                    else if (strcmp(circuit_string, "AND") == 0)
                    {
                        biwise_op = i == 0 ? binary_value_int : binary_value_int & biwise_op;
                    }
                    else if (strcmp(circuit_string, "NOR") == 0)
                    {
                        biwise_op = i == 0 ? binary_value_int : !(binary_value_int | biwise_op);
                    }
                    else if (strcmp(circuit_string, "XOR") == 0)
                    {
                        biwise_op = i == 0 ? binary_value_int : binary_value_int ^ biwise_op;
                    }
                }
                // printf("Updating %s and bitwise op %d\n", circuit_string, biwise_op);
                char *one_output_circuits = (char *)malloc(sizeof(char) * 17); // free

                read_variable(one_output_circuits, file_pointer_circuit); // reads output variable here

                update_table_by_string(table, one_output_circuits, biwise_op); // updates the output variable's binary value with the correct bitwise_op

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
                char *one_output_circuits = (char *)malloc(sizeof(char) * 17); // free
                read_variable(one_output_circuits, file_pointer_circuit);

                update_table_by_string(table, one_output_circuits, bitwise_op); // update output variable and it's binary number with bitwise_op

                free(one_input_circuits);
            }
            else if (strcmp(circuit_string, "DECODER") == 0)
            {
                char string_decoder_input_length[17];
                read_variable(string_decoder_input_length, file_pointer_circuit);

                int decoder_input_size = atoi(string_decoder_input_length);
                char **n_input_circuits = (char **)malloc(sizeof(char *) * decoder_input_size); // free
                int sum_binary = 0;
                for (int i = decoder_input_size - 1; i >= 0; i--)
                {
                    n_input_circuits[i] = (char *)malloc(sizeof(char) * 17); // free

                    read_variable(n_input_circuits[i], file_pointer_circuit);
                    int binary_value = index_table_by_string(table, n_input_circuits[i]);
                    sum_binary += (((int)pow(2, i)) * binary_value);
                }
                int decoder_output_size = (int)pow(2, decoder_input_size);
                char **n_output_circuits = (char **)malloc(sizeof(char *) * decoder_output_size); // free
                for (int j = 0; j < decoder_output_size; j++)
                {
                    n_output_circuits[j] = (char *)malloc(sizeof(char) * 17); // free
                    read_variable(n_output_circuits[j], file_pointer_circuit);

                    if (j == sum_binary)
                    {
                        update_table_by_string(table, n_output_circuits[j], true);
                    }
                    else
                    {
                        update_table_by_string(table, n_output_circuits[j], false);
                    }
                }
            }
            else if (strcmp(circuit_string, "MULTIPLEXER") == 0)
            {
                char string_multiplexer_selector_length[17];
                read_variable(string_multiplexer_selector_length, file_pointer_circuit);

                int selector_input_size = atoi(string_multiplexer_selector_length);
                int multiplexer_input_size = (int)pow(2, selector_input_size);

                char **n_input_circuits = (char **)malloc(sizeof(char *) * multiplexer_input_size); // free
                char **n_selector_circuits = (char **)malloc(sizeof(char *) * selector_input_size); // free

                for (int i = 0; i < multiplexer_input_size; i++)
                {
                    n_input_circuits[i] = (char *)malloc(sizeof(char) * 17);
                    read_variable(n_input_circuits[i], file_pointer_circuit);
                }
                int base_10_selector_value = 0;
                for (int j = selector_input_size - 1; j >= 0; j--)
                {
                    n_selector_circuits[j] = (char *)malloc(sizeof(char) * 17); // free
                    read_variable(n_selector_circuits[j], file_pointer_circuit);

                    int binary_value = index_table_by_string(table, n_selector_circuits[j]);
                    base_10_selector_value += (((int)pow(2, j)) * binary_value);
                }

                // printf("The binary base-10 is %d\n", base_10_selector_value);
                char *decoder_output_circuits = (char *)malloc(sizeof(char) * 17); // free
                read_variable(decoder_output_circuits, file_pointer_circuit);

                int binary_value = index_table_by_string(table, n_input_circuits[base_10_selector_value]);
                bool binary_rep = binary_value == 1 ? true : false;
                update_table_by_string(table, decoder_output_circuits, binary_rep); // update output variable and it's binary number with bitwise_op
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
        if(fseek(file_pointer_circuit, 0L, SEEK_SET) != 0){
            printf("%s\n", "Couldn't read the file at the given location!");
            return EXIT_FAILURE;
        }
    }
}

// handle circuit output
// char output_string[17];
// read_variable((char *)&output_string, file_pointer_circuit);
// if(strcmp(output_string, "OUTPUT") >=0){
//     read_variable(output_string, file_pointer_circuit);
//     int num_inputs = atoi(output_string);
//     for(int i =0; i < num_inputs;++i){
//         read_variable(output_string, file_pointer_circuit);
//         add_to_table(table, output_string);
//         // int hash_val = hash_value(output_string);
//         // printf("Output String: %s, Output ascii: %d\n", output_string, hash_val);
//         // table[hash_val] = false;
//     }
// }

// printf("%s\n", directive);

/*
        Reads: INPUT PR OS 
        Notice PR Ascii is the same as OS Ascii value of 162
        So in the table it should be chained together as PR OS
    */
//// Bad Code
// char input_string[17]; // This will read "INPUT"
// read_variable((char *)&input_string, file_pointer_circuit); // A wrapper function that just runs fscanf in the backend

// if(strcmp(input_string, "INPUT") >=0){
//     read_variable(input_string, file_pointer_circuit);
//     int num_inputs = atoi(input_string); // reads the number of inputs
//     for(int i =0; i < num_inputs; i++){
//         read_variable(input_string, file_pointer_circuit); // reads the single string into the input_strs[i]
//         add_to_table(table, input_string); // add input_strs[i] to the main hash table.
//     }
// }
// print_list(table[162]); // Prints OS OS