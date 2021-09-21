#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node;
struct student;
typedef struct node node;
typedef struct student student;
int myfscanf(FILE *f,const char* format, node *headnode);

struct student{ //students structs
    char *name;
    char initial;
    char *surname;
    int year;
    char *course_name;
    char group;
    int average;
};

void print_student_details(const student *s){ //printing students struct
    printf("Name: %s  Initial: %c  Surname: %s  Year: %d Course name:%s  Group: %c Average: %d\n",s->name,s->initial,s->surname,s->year,s->course_name,s->group,s->average);
}

student *new_student(){ //creating a new student
    student *s;
    s =malloc(sizeof (student)); //allocating space for the student record
    return s;
}

enum{ //storing the possible values for the node
    TYPE_INT,
    TYPE_CHAR,
    TYPE_STRING,
    TYPE_STUDENT,
};

struct node{ //node struct
    int data_type;
    void *data;
    node *next_node;
};

node* new_node(void *data, int type){ //creating a new struct
    node *n;
    n = malloc(sizeof (node)); //allocating space for list node

    n->data_type = type;
    n->data = data;
    n->next_node = NULL;

    return n;
}

int myfscanf(FILE *f,const char* format, node *headnode){ //fscanf implementation
    int noParameters = 0; //number of format string
    unsigned int i = 0; //index of char
    char char_from_file;
    node *node = headnode;

    while ((char_from_file = (char)fgetc(f))){ //reading chars form files
        if(char_from_file == EOF){
            if(!noParameters){
                return EOF;
            }else{
                return noParameters;
            }
        }
        if(char_from_file =='\n') return noParameters;

        if(format[i++] == '%'){ //looking for a % followed by a :
            switch(format[i]){
                case 'c': //char
                {
                    node->data = malloc(sizeof(char_from_file));
                    *(char*)node->data = char_from_file;
                    node->data_type = TYPE_CHAR;
                }
                    break;
                case 'd'://int
                {
                    int int_from_file = 0;
                    while (1) {
                        if ((char_from_file <= 57) && (char_from_file >= 48)) { //turn string to int
                            int char_to_int = char_from_file - '0';
                            int_from_file *= 10;
                            int_from_file += char_to_int;
                            char_from_file = (char)fgetc(f);
                        } else {
                            ungetc(char_from_file, f);
                            break;
                        }
                    }

                    node->data = malloc(sizeof(int));
                    *(int*)node->data = int_from_file;
                    node->data_type = TYPE_INT;
                }
                    break;
                case 's':
                {
                    char string_from_file[100] = "";
                    while((char_from_file = (char)fgetc(f))){
                        if(char_from_file == '\n' || char_from_file == ' ' || char_from_file ==EOF){
                            ungetc(char_from_file,f);
                            break;
                        }
                        strcat(string_from_file, &char_from_file);
                    }
                    node->data = malloc(sizeof(string_from_file));
                    strcpy(node->data, string_from_file);
                    node->data_type = TYPE_STRING;
                }
                    break;
                default:
                    printf("error");
                    return noParameters;
            }
            noParameters++;
            node->next_node = new_node(NULL, -1);
            node = node->next_node;
            i++;
            continue;
        }

    }

    return 0;
}

int main(int argc, char  *argv[]) {
    node list_head; //start of linked list
    list_head.next_node = NULL;

    if(argc < 2){ //making sure that command line argument for the file exists
        printf("The students text File  is missing!");
    }else{
        FILE *f; //reference to the file
        if((f = fopen(argv[1], "r")) == NULL){ //checking if the file exists
            printf("There was a problem while opening the file\n");
        } else {
            printf("file opened");
        }

        node *students_head = new_node(NULL, -1);
        node *current_student_node = students_head;
        node *student_info = new_node(NULL, -1);
        while(myfscanf(f, "%s (%c) %s %d %s %c %d", student_info)) {
            current_student_node = new_node(new_student(), TYPE_STUDENT);
            student *s = current_student_node->data;
            s->name = student_info->data;
            student_info = student_info->next_node;
            s->initial = *(char*)student_info->data;
            student_info = student_info->next_node;
            s->surname = student_info->data;
            student_info = student_info->next_node;
            s->year = *(int*)student_info->data;
            student_info = student_info->next_node;
            s->course_name = student_info->data;
            student_info = student_info->next_node;
            s->group = *(char*)student_info->data;
            student_info = student_info->next_node;
            s->average = *(int*)student_info->data;
            print_student_details(s);

            current_student_node->next_node = new_node(new_student(), TYPE_STUDENT);
        }
        printf("end of file\n");
        fclose(f);
    }
    return 0;
}