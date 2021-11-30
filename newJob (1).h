#define _CRT_SECURE_NO_WARNINGS
#define INVALID -1
#define LOGOUT 3
#define OUT 5
#define MAX_STR 20

#define F_NAME 11
#define L_NAME 11 
#define ID 10
#define PHONE 11 
#define AGE 3
#define PASS 9
#define PURPOSE 2

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef enum { F, T } boolean;

struct user
{
	char f_name[MAX_STR];
	char l_name[MAX_STR];
	char id[MAX_STR];
	char phone[MAX_STR];
	char age[MAX_STR]; 
	char pass[MAX_STR];
	char purpose[MAX_STR];
};

struct login
{
	char id[ID];
	char pass[PASS];
};

struct job 
{
	int employer;
	int profession;
	int location;
	int type;
	int experience;
	int candidate1 ;
	int candidate2 ;
	int candidate3 ;
	int total ;
};

//transfer files to arrays - start
int get_users_size();
struct user* user_arr(int size);
struct login* login_arr(int size);
int get_job_size();
struct job* jobs_arr(int size);

//login and registration
int login_page();
int sign_in_page(struct login* arr, int size);
void join_page(struct user * newUser);

//main menu options
int main_menu(char * purpose);
void edit_profile(struct user* user);
int select_filters(int *j_filter);
int print_relevant_jobs(int j_filter, int sub_filter, struct job * jobs, int j_size, char * purpose);
void print_job(struct job * job, int index);
boolean submit_profile(int index, int j_filter, int sub_filter, struct job * job);
void add_profile(int index, struct job * job);
void aptitude_test();
void publish_job(struct job * job, int index);
void submissions_history(int index, struct job * jobs, int j_size);
int view_all_my_jobs(int index, struct job ** jobs, int j_size);
void update_job(struct job *job);
void delete_job(struct job * job);
void  print_user(struct user c);

// check input
int type_number(int start, int end);
boolean isNumber(const char* s);
int isdigit(char c);
boolean checkAge(char* age);
void get_str(char * str, size_t start, size_t end, boolean only_numbers);
boolean id_exist(char * id, struct login * login, int size);

//write to files - end 
void write_to_size(int size);
void write_to_users(struct user* users, int size);
void write_to_login(struct login* login, int size);
void write_to_jsize(int size);
void write_to_jobs(struct job* jobs, int size);








