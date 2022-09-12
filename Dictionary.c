
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<conio.h>
#define w_size 25
#define m_size 250
char word_list[100];
int cursor_pos = 0;
struct Tree
{
    char arr[w_size];
    char meaning[m_size];
    struct Tree *left;
    struct Tree *right;
};

struct Tree *get_node(char word[],char meaning[])
{
    struct Tree *new_node = (struct Tree *)malloc(sizeof(struct Tree));
    strcpy(new_node->arr,word);
    strcpy(new_node->meaning,meaning);
    new_node->left = new_node->right = NULL;
    return new_node;
}


struct Tree *insert(struct Tree *root,char word[],char meaning[])
{
    if(root == NULL)
    {
        root = get_node(word,meaning);
    }
    else
    {
        if(strcmp(root->arr,word) < 0)
        {
            root->right = insert(root->right,word,meaning);
        }
        else
        {
            root->left = insert(root->left,word,meaning);
        }
    }
    return root;
}
struct Tree *smallest_ascii(struct Tree *root)
{
    while(root->left != NULL)
        root = root->left;
    return root;
}
struct Tree *Delete(struct Tree *root,char word[])
{
    struct Tree *store;
    if(strcmp(root->arr,word) < 0)
        root->right = Delete(root->right,word);
    else if(strcmp(root->arr,word) > 0)
        root->left = Delete(root->left,word);
    else
    {
        if(root->left == NULL && root->right == NULL)
        {
            free(root);
            root = NULL;
        }
        else if(root->left == NULL)
        {
            free(root);
            root = root->right;
        }
        else if(root->right == NULL)
        {
            free(root);
            root = root->left;
        }
        else
        {
            store = smallest_ascii(root->right);
            strcpy(root->arr,store->arr);
            strcpy(root->meaning,store->meaning);
            free(store);
            store = NULL;
        }
    }
    return root;
}
int search(struct Tree *root,char word[]){
    if(root == NULL)
        return 0;
    else if(strcmp(root->arr,word) == 0)
        return 1;
    else if(strcmp(root->arr,word) < 0)
        return search(root->right,word);
    else
        return search(root->left,word);
}
void FILE_input(char array[],char m_array[])
{

    char s;
    int i = 0;
    FILE *fp;
    fp = fopen("Dictionary_2.0.txt","r");
    fseek(fp,cursor_pos,0);
    s = fgetc(fp);
    while((s != '~') && (s != EOF) && (i != w_size))
    {
        array[i] = s;
        i++;
        s = fgetc(fp);
    }
    if(i != w_size)
        array[i] = '\0';
    i = 0;
    s = fgetc(fp);
    while((s != '\n') && (s != EOF) && (i != m_size))
    {
        m_array[i] = s;
        i++;
        s = fgetc(fp);
    }
    if(i != m_size)
        m_array[i] = '\0';
    cursor_pos = ftell(fp) - 1;
    fclose(fp);
}
struct Tree *load_BST(struct Tree *root)
{
    FILE *fp;
    char temp[w_size];
    char m_temp[m_size];
    int c;
    fp = fopen("Dictionary_2.0.txt","r");
    c = fgetc(fp);
    cursor_pos = ftell(fp);
    while(c != EOF)
    {
        if(c == '\n')
        {
            FILE_input(temp,m_temp);
            root = insert(root,temp,m_temp);
        }
        fseek(fp,cursor_pos,0);
        c = fgetc(fp);
        cursor_pos = ftell(fp);
    }
    fclose(fp);
    return root;
}
void display(struct Tree *root)
{
    if(root != NULL)
    {
        display(root->left);
        printf("%s = %s\n",root->arr,root->meaning);
        display(root->right);
    }
}
void file_insert(char word[],char word_user[],char meaning[])
{
    FILE *fp;
    fp = fopen("Dictionary_2.0.txt","a");
    fprintf(fp,"\n");
    fprintf(fp,word);
    fputc('~',fp);
    fputs(meaning,fp);
    fclose(fp);
}
void find_meaning(struct Tree *root,char word[])
{
    if(root == NULL)
        return;
    else if(strcmp(root->arr,word) == 0)
        printf("\nThe meaning is : %s",root->meaning);
    else if(strcmp(root->arr,word) < 0)
        find_meaning(root->right,word);
    else
        find_meaning(root->left,word);
}
int line_number(char word[])
{
    int c,line=0;
    char words[w_size],m[m_size];
    FILE *fp;
    fp = fopen("Dictionary_2.0.txt","r");
    while((c=fgetc(fp)) != EOF)
    {
        if(c == '\n')
        {
            line++;
            FILE_input(words,m);
            if((strcmp(word,words)) == 0)
                break;
        }
    }
    fclose(fp);
    return line;
}
void del_from_file(char word[])
{
    FILE *fp1,*fp2;
    int c,line,count=0;
    line = line_number(word);
    fp1 = fopen("Dictionary_2.0.txt","r");
    fp2 = fopen("Temporary.txt","w");
    while((c=fgetc(fp1)) != EOF)
    {
        if(c == '\n')
        {
            count++;
        }
        if(count != line)
        {
            fputc(c,fp2);
        }
    }
    fclose(fp1);
    fclose(fp2);
    remove("Dictionary_2.0.txt");
    rename("Temporary.txt","Dictionary_2.0.txt");
}
void arrange_words_in_FILE(struct Tree *root)
{
    if(root != NULL)
    {
        arrange_words_in_FILE(root->left);
        FILE *fp;
        fp = fopen("arrange.txt","a");
        fprintf(fp,"\n%s",root->arr);
        fclose(fp);
        arrange_words_in_FILE(root->right);
    }
}
void send_words_from_FILE(int n,char word[])
{

    int i,len,c,count = 0,max_word_count = 0;
    int x;
    char s;
    char new_word[w_size];
    FILE *fp;
    fp = fopen("arrange.txt","r");
    strupr(word);
    word_list[0] = '\0';
    s = fgetc(fp);
    if(n == 0)
    {
        return;
    }
    while(s != EOF)
    {
        if(s == '\n')
        {
            i = 0;
            while((s=fgetc(fp)) != '\n')
            {
                new_word[i] = s;
                i++;
            }
            new_word[i] = '\0';
            c = 0;
            for(i=0; i<n; i++)
            {
                if(word[i] == new_word[i])
                    c++;
            }
            if(c == n)
            {
                max_word_count = max_word_count + 1;
                len = strlen(new_word);
                for(i=0; i<len; i++)
                {
                    word_list[count] = new_word[i];
                    count++;
                }
                word_list[count] = ',';
                count++;
            }
        }
        new_word[n] = '\0';
        if((max_word_count == 4) || (strcmp(word,new_word) < 0))
        {
            break;
        }
    }
    word_list[count-1] = '\0';
    fclose(fp);
}
void user_input(struct Tree *root,char array[],int size,char array_user[])
{
    int ch,i;
    FILE *fp;
    fp = fopen("arrange.txt","w");
    fclose(fp);
    arrange_words_in_FILE(root);
    for(i=0; i<size; i++)
    {
        ch = getch();
        printf("\r                                                                               ");
        if(ch == 8)
        {
            array[i-1] = '\0';
            array_user[i-1] = '\0';
            send_words_from_FILE(i-1,array);
            printf("\rEnter word :%s         %s",array_user,word_list);
            i = i-2;
        }
        else if(ch == 13)
        {
            array[i] = '\0';
            array_user[i] = '\0';
            printf("\rEnter word :%s",array_user);
            break;
        }
        else if(ch == 32)
        {
            if(i==0 || isspace(array[i-1]))
                i = i-1;
            else
            {
                array[i] = ch;
                array_user[i] = ch;
                array[i+1] = '\0';
                array_user[i+1] = '\0';
            }
            printf("\rEnter word :%s         %s",array_user,word_list);
        }
        else if((ch >= 91 && ch <= 96) || (ch >= 123 && ch <= 126))
        {
            array[i] = ch;
            array_user[i] = ch;
            array[i+1] = '\0';
            array_user[i+1] = '\0';
            if(ch == 95)
            {
                send_words_from_FILE(i+1,array);
            }
            printf("\rEnter word :%s         %s",array_user,word_list);
        }
        else
        {
            array[i] = ch;
            array_user[i] = ch;
            array[i+1] = '\0';
            array_user[i+1] = '\0';
            send_words_from_FILE(i+1,array);
            printf("\rEnter word :%s         %s",array_user,word_list);
        }
        if(i < 0)
        {
            i = -1;
            printf("\r                                                                           ");
            printf("\rEnter word :");
        }
    }
    remove("arrange.txt");
}

int main()
{
    struct Tree *root=NULL;
    int ch;
    int s;
    char word[w_size],word_user[w_size];
    word_list[0] = '\0';
    root = load_BST(root);
    printf("\n\t\t\t\tDSC project- DICTIONARY\n\t\t\t\t\t--- By Kausthubh K, Khushi Agrawal, Vignesh J R(ISE-B)\n\n");


    while(ch != '5')
    {

        printf("Press   1 To search a word's meaning\n\t2 To add a word\n\t3 To delete a word\n\t4 To edit a word's meaning\n\t5 To Exit\n\n\n");
        printf("Enter your choice : ");
        fflush(stdin);
        ch = getchar();
        switch(ch)
        {
        case '1':
            printf("Enter word :");
            user_input(root,word,24,word_user);
            if(search(root,word) == 0)
            {
                printf("\nEntered word is not listed in Dictionary.\n\t--Do you want to add this word ?");
                s = 0;
                do
                {
                    printf("\n\t--Press Y for YES or N for NO : ");
                    while((getchar()) != '\n');
                    s = getchar();
                    if(s == 'Y' || s == 'y')
                    {


                        char meaning[m_size];
                        printf("\nEnter the meaning of %s : ",word_user);
                        while((getchar()) != '\n');
                        gets(meaning);
                        root = insert(root,word,meaning);
                        file_insert(word,word_user,meaning);
                        printf("\n \" %s \" is added to the Dictionary.",word_user);


                    }
                    else if(s == 'N' || s == 'n')
                        printf("\n \" %s \" is not added in dictionary.\n\n",word_user);
                }
                while(s != 'N' && s != 'n');
            }
            else
            {
                find_meaning(root,word);
            }
            break;
        case '2':


            printf("Enter word :");
            user_input(root,word,24,word_user);
            if(search(root,word) == 0)
            {
                char meaning[m_size];
                printf("\nEnter the meaning of %s : ",word_user);
                while((getchar()) != '\n');
                gets(meaning);
                root = insert(root,word,meaning);
                file_insert(word,word_user,meaning);
                printf("\n \" %s \" is added to the Dictionary.",word_user);
            }
            else
                printf("\n\"%s\" already exists in Dictionary.\n",word_user);

            break;
        case '3':


            printf("Enter word :");
            user_input(root,word,24,word_user);
            if(search(root,word) == 0)
                printf("Entered word is not listed in Dictionary.\n\n");
            else
            {
                printf("\n\nThe word will be deleted permanently from the Dictionary.\n\n\tDo you want to delete this word ?");
                s = -1;
                while(s != 'N' && s != 'n')
                {
                    printf("\n\t--Press Y for YES or N for NO : ");
                    fflush(stdin);
                    s = getchar();
                    if(s == 'Y' || s == 'y')
                    {
                        root = Delete(root,word);
                        del_from_file(word);
                        printf("\" %s \" is deleted from the Dictionary.\n",word_user);
                        break;
                    }
                    else if(s == 'N' || s == 'n')
                        printf("\" %s \" is not deleted from the Dictionary.\n",word_user);
                }
            }
            break;
        case '4':


            printf("Enter word :");
            user_input(root,word,24,word_user);
            if(search(root,word) == 0)
                printf("Entered word is not listed in Dictionary.\n\n");
            else
            {
                root = Delete(root,word);
                del_from_file(word);
                char meaning[m_size];
                printf("\nEnter the meaning of %s : ",word_user);
                while((getchar()) != '\n');
                gets(meaning);
                root = insert(root,word,meaning);
                file_insert(word,word_user,meaning);

                printf("The meaning of \" %s \" is edited.\n",word_user);
            }

            break;
        case '5':
            printf("\nThanks for using the dictionary");
            break;
        default :
            printf("Please press any of the numbers from the MENU\n\n\n");
        }
        printf("\n\n\n");
    }
}
