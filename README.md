# soal-shift-sisop-modul-3-I01-2021

1. Keverk is a famous ambitious person in his colleagues. Before he became the head of department in HTMC, he has done a project which tells him to make a book server database. This project is required, so that it could be used by the app owner and is expected from the users. 

In this project, Keverk is asked to: 

When a client is connected to the server, there are two main options. They are register and login. If the user has chosen the register option, the client will prompt an id and password to be sent to the server. Users can also login to the server. Login is successful if the id and the password sent to the server matched with one of the accounts listed on the server application. This system can accepts multiple connections at once. A connection is counted when a client application is connected to the server. If there are 2 connections or more then we must wait for the first client to exit the application to be able to login and use the application. Keverk wanted the file that stores the id and password named account.txt with the following format :

akun.txt
id:password
id2:password2


The system has a database on a file named files.tsv. The content of the file is the file path on the server, publisher, and the year of publication. For every insertion and removal of a file in the folder FILES on the server, the file files.tsv will be affected. Folder FILES are made, automatically when the server is started

Not only that, Keverk is asked to make a feature such that clients are able to insert new files to the server. FILES directory have a structure as follows : 

Direktori FILES 
File1.extension
File2.extension


First, the client send input to the server with the structure as follows:
		Client command example :
		
add


			

		Client Console Output:
		
Publisher:
Publication year:
Filepath:


Then, we can fill the book’s data (please note that Filepath is the path to the file that will be sent to the server). Then the client will send the file to the server via socket. When a file is received at the server, a new row containing new data of the book is then added to the files.tsv.

Clients can also download files that exist in the FILES folder in the server, then the system must send the following file to the client. Server must check from files.tsv to check if the file requested is valid. If it’s not valid, the system sends an error message back to the client. If valid, the following file is then sent to the client in the client folder.

Client command example
download TEMPfile.pdf



After that, the client can also delete file that is stored on the server. But keverk is afraid that the file that is going to be deleted is an important file, so instead of deleting the file, the program just changes its name to ‘old-FileName.extension’. When the filename is changed, the row of the file in files.tsv will be deleted.

Client command example:

delete TEMPfile.pdf



Clients can see all the contents of files.tsv by calling the see command. Output of the command will follow this format. 

Client command example :

see


Client Console Output:
Name:
Publisher:
Publication year:
File extension : 
Filepath : 

Name:
Publisher:
Publication Year:
File extension: 
Filepath : 





The client application can also do search by using the find command and a string. It will output the same format as number 6.

Client command example:

find TEMP



Keverk is cautious on insertion and removal of files on the server, so he made a log file for the server named running.log. The content of the log file is following this format

running.log
Tambah : File1.extension (id:pass)
Hapus : File2.extension (id:pass)



But keverk has not yet understood the client and server system. Help Keverk to finish this project!

Note: 
It is not allowed to use system() dan execv(). Please do the task fully using thread and socket programming. 
To download and upload, please use any text file with extension. The content of the file can be anything. (Please don’t use an empty file) and the accepted requirement is trial with the minimum of 5 data.


Directory Structure:

.
├── Client
│   ├── client.c
│   ├── File2.extension
│   └── File1.extension
└── Server
    ├── account.txt
    ├── files.tsv
    ├── server.c
    ├── running.log
    └── FILES
            ├── File2.extension
            └── File1.extension




2. Crypto (you) is Loba's friend. One morning, Crypto saw Loba who was overwhelmed with his boss's assignment. Since Crypto is such a challenging person, he wanted to help Loba with his job. The details of the assignment are:
Create a matrix multiplication program (4x3 and 3x6) and then display the results. The matrix will contain the numbers 1-20 (no need to create a number filter).
Create a program using the output matrix of the previous program (program soal2a.c) (Note!: Use shared memory). Then the matrix will be calculated with the new matrix. As follows an example of calculation for the existing matrix. The calculation is that each cell originating from matrix A becomes a number for factorial, then cells from matrix B become the maximum factorial limit (from largest to smallest) (Note!: Use threads for calculations in each cell). 

		Provisions
		
If a >= b  -> a!/(a-b)!
If b > a -> a!
If 0 -> 0


Example :

A
B
Factorial numbers
4
4
4 3 2 1
4
5
4 3 2 1
4
3
4 3 2
4
0
0
0
4
0
4
6
4 3 2 1

		
		Example :

Matrix A
Matrix B
Result Matrix
0
4
0
4
0
4*3*2*1
4
5
6
2
4*3*2*1
5*4
5
6
6
0
5*4*3*2*1
0


For fear of lags in the process of helping Loba, Crypto also created a program (soal2c.c) to check the top 5 processes consuming computer resources with the command  “ps aux | sort -nrk 3,3 | head -5” (Note !: You must use IPC Pipes)

	Notes:
All matrices come from the input to the program.
Prohibited from using system ()


3. During his time of inactivity, a student named Alex. He had an idea to tidy up a number of files on his laptop. Because there are too many files, Alex asked Ayub for advice. Ayub suggests creating a C program to categorize the files. This program will move files according to their extension into a folder according to the extension. The folder is in the working directory in which the categorization program is run. 

Example:
# The category program is located at /home/izone/kategori
$ ./kategori -f path/to/file1.jpg path/to/file2.c path/to/file3.zip
#Result
/home/izone
|-jpg
|--file1.jpg
|-c
|--file2.c
|-zip
|--file3.zip


a. Program accepts -f option like stated above, with this option the user may add file arguments to be categorized as much as they want. 
Output:
File 1 : Berhasil Dikategorikan (jika berhasil)
File 2 : Sad, gagal :( (jika gagal)
File 3 : Berhasil Dikategorikan

    getcwd(cwd, sizeof(cwd));

    int i=0;
    //loop sebanyak argumen file trus masuk ke thread yg dibuat buat mindahin file
    if (strcmp(argv[1],"-f") == 0) 
    {
        for(int j = 2 ; j < argc ; j++ )
        {
            pthread_create(&(tid[i]),NULL,function,argv[j]);
            pthread_join(tid[i],NULL);
            printf("File %d : Berhasil Dikategorikan\n", i+1);
            i++;
        }
    }


b. Program may accept -d option to categorize a directory instead. With this option, the user may only input 1 directory as it's arguments, unlike the -f option where the user may input multiple file arguments. Example:
$ ./kategori -d /path/to/directory/

The command above will categorize the files in /path/to/directory, the categorization result will be saved in the working directory where the C program is called (categorization result is not located at /path/to/directory).
Output:
Jika berhasil, print “Direktori sukses disimpan!”
Jika gagal, print “Yah, gagal disimpan :(“

    else if (strcmp(argv[1],"-d") == 0 && argc == 3) 
    {
        i = 0;
        DIR *fd;
        struct dirent *masuk;
        char tempata[100];
        fd = opendir(argv[2]);
        //ngecek bisa dibuka apa engga direktorinya
        if(fd == NULL)
        {
            printf("Yah, gagal disimpan :(\n");
        }
        //ngebaca apa aja yg ada di direktorinya
        while( (masuk=readdir(fd)) )
        {
            if ( !strcmp(masuk->d_name, ".") || !strcmp(masuk->d_name, "..") )
            continue;

            //nyimpen path file yg dikategoriin di tempata
            strcpy(tempata,argv[2]);
            strcat(tempata,"/");
            strcat(tempata,masuk->d_name);
            //ngecek file bukan kalo iya masuk ke thread
            if(masuk->d_type == 8)
            {
            pthread_create(&(tid[i]),NULL,function,tempata); //bikin thread
            pthread_join(tid[i],NULL);
            i++;
            }
        }
        printf("Direktori Sukses Disimpan!\n");
    }


c. Other than the above options, this program accepts the * option. Example:
$ ./kategori \*

This option categorizes all the file in the working directory when the C program is run

    else if (strcmp(argv[1],"*") == 0 && argc == 2) 
    {
        i = 0;
        DIR *fd;
        struct dirent *masuk;
        char tempata[100];
        fd = opendir(cwd);

        if(fd == NULL)
        {
            printf("Yah, gagal disimpan :(\n");
        }
        char tempatsoal[100] = "/home/damdum/Soal3/soal3.c", tempatsoal3[100] = "/home/damdum/Soal3/soal3";
        while((masuk=readdir(fd)))
        {
            if (!strcmp(masuk->d_name, ".") || !strcmp(masuk->d_name, "..") )
            continue;
            
            strcpy(tempata,cwd);
            strcat(tempata,"/");
            strcat(tempata,masuk->d_name);
            //printf("tempata = %s\n", tempata);
            //printf("tempatsoal = %s\n", tempatsoal);
            //printf("tempatsoal3 = %s\n", tempatsoal3);
            if((strcmp(tempata,tempatsoal) == 0) || strcmp(tempata,tempatsoal3) == 0) 
            continue;
            
            else if(masuk->d_type == 8){
            pthread_create(&(tid[i]),NULL,function,tempata); //membuat thread
            pthread_join(tid[i],NULL);
            i++;
            }
        }
        printf("Direktori suskses disimpan!\n");
    }

d. All files must be moved into a folder. Files without extensions will be moved to a folder named "Unknown". Hidden files will be moved to a folder named "Hidden".

    else
    {
        strcpy(temp,cwd);
        strcat(temp,"/");
        strcat(temp,"Unknown");
        mkdir(temp, 0777);
    }
	

    char path[1024], destination[1024];
    strcpy(path,arg);
    strcpy(destination,cwd);
    strcat(destination,"/");
    if(n == 1 )
    {
        strcat(destination,"Unknown");
    }
    else
    {
        strcat(destination,lowcase);
    }
    strcat(destination,"/");
    strcat(destination,arr3);
    rename(path,destination);
    n = 0;
    m = 0;

	return NULL;

e. Each file to be categorized will be processed by a thread to make the program run in parallel to make it quicker.

But because Ayub is simply an idea man and doesn't want to help build the program, Ayub is asking your help to create the program.

Note: 
Category folder is not created manually, it must be created with the C program
This program is case insensitive. Example: JPG and jpg is treated the same
If the extension consists of more than one (example “.tar.gz”) the file will be put in a folder with the formermost dot character (example “tar.gz”)
Fork-exec and system() functions are not allowed
Code for section b and c must be recursive in design
