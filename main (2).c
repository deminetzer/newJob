#include "newJob.h"

int main()
{
	int u_size = get_users_size(); //number of users
	struct user* users = user_arr(u_size); //users = users array
	if ((!users)&&(u_size!=0)) {printf("error"); return 0; }
	struct login* login = login_arr(u_size); //login = array of login data (password and id)
	if ((!login) && (u_size != 0)) { printf("error"); return 0; }
	int j_size = get_job_size(); //number of jobs
	struct job* jobs = jobs_arr(j_size); //jobs = jobs array
	if ((!jobs) && (j_size != 0)) { printf("error"); return 0; }
	
	int login_select = 0; //login menu
	int menu_select = 0; // main menu
	int j_filter, sub_filter; //job search filters
	int u_index; //current user
	int j_index; //current job 
	int j_update; 
	
	printf("~ Wellcome to NewJob ~");
	while (login_select != LOGOUT)
	{
		menu_select = 0;
		login_select = login_page();
		switch (login_select)
		{
		case 1:
			u_index = sign_in_page(login, u_size);
			if (u_index == INVALID) { printf("\nIncorrect ID number and / or password !\n"); break; }
			printf("\nDetails verified successfully !\n ");
			while (menu_select != OUT) {
				menu_select = main_menu(users[u_index].purpose);
				switch (menu_select) {
				case 1:
					edit_profile(users + u_index); 
					break; 
				case 2: 
					sub_filter = select_filters(&j_filter);
					j_index = print_relevant_jobs(j_filter, sub_filter, jobs, j_size, users[u_index].purpose);
					if ((strcmp(users[u_index].purpose, "1") == 0)) {
						if ((j_index >= 0) && (j_index < j_size)) {
							if(submit_profile(u_index, j_filter, sub_filter, jobs + j_index)==F)
								printf("\nInvalid job number ! Back to main menu\n");
						}
						else { printf("\nProfile not submitted, back to main menu\n"); }
					}
					break;
				case 3:
					if (strcmp(users[u_index].purpose, "1") == 0)
						aptitude_test();
					else {
						j_size++;
						jobs = (struct job*)realloc(jobs, (j_size) * sizeof(struct job));
						if (!jobs) { printf("Error!"); return 0; }
						publish_job(jobs + j_size - 1,u_index);	
					}
					break;
				case 4:
					if (strcmp(users[u_index].purpose, "1") == 0) {
						submissions_history(u_index, jobs, j_size);
					}
					else {
						j_update = view_all_my_jobs(u_index, &jobs, j_size);
						if (j_update == INVALID) break;
						if (jobs[j_update].total == 0) { printf("Sorry! No candidates were found for this job \n"); break; }
						printf("\n~View Candidates profiles\n");
						if (jobs[j_update].candidate1 != INVALID) { print_user(users[jobs[j_index].candidate1]); }
						if (jobs[j_update].candidate2 != INVALID) { print_user(users[jobs[j_index].candidate2]); }
						if (jobs[j_update].candidate3 != INVALID) { print_user(users[jobs[j_index].candidate3]); }
					}
					break;
				} 
			}
			break;
		case 2:
			u_size++;
			users = (struct user*)realloc(users, (u_size) * sizeof(struct user));
			if (!users) { printf("Error!"); return 0; }
			join_page(users + u_size - 1); 
			login = (struct login*)realloc(login, (u_size) * sizeof(struct login)); 
			if (!login) { printf("Error!"); return 0; }
			while (id_exist(users[u_size-1].id, login, u_size-1)==T)
			{
				printf("\nERROR ! The ID number already exists in the system, type a new number \n");
				get_str(users[u_size - 1].id, ID - 1, ID - 1, T);
			}
			strcpy(login[u_size - 1].id, users[u_size-1].id);
			strcpy(login[u_size - 1].pass, users[u_size-1].pass);
			printf("\nYou have registered successfully! Please connect to the system");
			break; 
		}
	}
	printf("\nGoodbye :)\n");
	write_to_size(u_size);
	write_to_users(users, u_size);
	write_to_login(login, u_size);
	write_to_jsize(j_size);
	write_to_jobs(jobs, j_size);

	free(jobs);
	free(users);
	free(login);

	return 0;
}


//transfer files to arrays - start
int get_users_size()
{
	FILE* sz = NULL;
	sz = fopen("sz1.txt", "r");  if (!sz) { printf("ERROR ! "); return 0; }
	int size; fscanf(sz, "%d", &size);
	fclose(sz);
	return size;
}

struct user* user_arr(int size)
{
	struct user* users = NULL;
	users = (struct user*)malloc(size * sizeof(struct user));
	if (!users) { return NULL; }
	FILE* usr = NULL;
	usr = fopen("users1.txt", "r"); if (!usr) { return NULL; }
	for (int i = 0; i < size; i++)
	{
		fscanf(usr, "%s %s %s %s %s %s %s ", users[i].f_name, users[i].l_name, users[i].id,
			users[i].phone, users[i].age, users[i].pass, users[i].purpose);
	}
	fclose(usr);
	return users;
}

struct login* login_arr(int size)
{
	struct login* login = NULL;
	login = (struct login*)malloc(size * sizeof(struct login));
	if (!login) { return NULL; }
	FILE* lg = NULL;
	lg = fopen("login1.txt", "r"); if (!lg) { return NULL; }
	for (int i = 0; i < size; i++)
	{
		fscanf(lg, "%s %s ", login[i].id, login[i].pass);
	}
	fclose(lg);
	return login;
}

int get_job_size()
{
	FILE* sz = NULL;
	sz = fopen("jsz1.txt", "r");  if (!sz) { printf("ERROR ! "); return 0; }
	int size; fscanf(sz, "%d", &size);
	fclose(sz);
	return size;
}

struct job* jobs_arr(int size)
{
	struct job* jobs = NULL;
	jobs = (struct job*)malloc(size * sizeof(struct job));
	if (!jobs) { return NULL; }
	FILE* jb = NULL;
	jb = fopen("jobs1.txt", "r"); if (!jb) { return NULL; }
	for (int i = 0; i < size; i++)
	{
		fscanf(jb, "%d %d %d %d %d %d %d %d %d ", &jobs[i].employer, 
			&jobs[i].profession, &jobs[i].location,
			&jobs[i].type, &jobs[i].experience, &jobs[i].candidate1, 
			&jobs[i].candidate2, &jobs[i].candidate3, &jobs[i].total);
	}
	fclose(jb);
	return jobs;
}


//login and registration
int login_page() 
{
	printf("\n\n~ Login and registration\n1. Sign in (for registered users)\n2. Join (for new users)\n3. Quit\n\n");
	return type_number(1,3);
} 

int sign_in_page(struct login* arr, int size)
{
	char id[ID];
	char pass[PASS];
	printf("\n~ Login\nPlease enter your : \nID number (9 digits) : "); scanf("%s", id);
	printf("Password (6 - 8 digits) : "); scanf("%s", pass);
	for (int i = 0; i < size; i++)
	{
		if (strcmp(arr[i].id, id) == 0)
			if (strcmp(arr[i].pass, pass) == 0) return i;
	}
	return INVALID;
}

void join_page(struct user * newUser)
{
	printf("\n~Welcome to the registration process, Please fill in all the fields as per the requirements \n");
	printf("first name (up to 10 characters) : "); 
	get_str(newUser->f_name, 1, F_NAME - 1, F);
	printf("last name (up to 10 characters) : "); 
	get_str(newUser->l_name, 1, L_NAME - 1, F);
	printf("ID number (9 digits) : "); 
	get_str(newUser->id, ID - 1, ID - 1, T);
	printf("phone number (10 digits) : ");
	get_str(newUser->phone, PHONE - 1, PHONE - 1, T);
	printf("age (16 - 70) : "); scanf("%s", newUser->age);
	while (checkAge(newUser->age) == F) 
	{ printf("invalid input ! try again \n"); scanf("%s", newUser->age); }
	printf("password (6 - 8 digits) : "); 
	get_str(newUser->pass, PASS - 3, PASS - 1, T);
	printf("purpose (candidate - type 1, employer - type 2) : "); scanf("%s", newUser->purpose);
	while ((strcmp(newUser->purpose,"1")!=0)&&(strcmp(newUser->purpose, "2")!=0))
	{ printf("invalid input ! try again "); scanf("%s", newUser->purpose); }
} 


//main menu options
int main_menu(char * purpose) 
{
	printf("\n~ Main menu\n");
	printf("1. Edit profile\n");
	printf("2. Job search\n");
	if (strcmp(purpose, "1") == 0) {
		printf("3. Aptitude test\n"); //candidate
		printf("4. Submissions history\n");
	}
	else {
		printf("3. Publish new job\n"); //employer
		printf("4. View all my jobs\n");
	}
	printf("5. Log Out\n");	
	return type_number(1,5);
}

void edit_profile(struct user* user)
{
	char str[MAX_STR];
	printf("\n\n~ Edit profile \nType 'yes' for a field you want to change\n");
	
	printf("update your first name (%s) ?", user->f_name);  scanf("%s",str);
	if (strcmp(str, "yes") == 0) {
		printf("Enter new first name (up to 10 characters) : ");
		get_str(user->f_name, 1, F_NAME - 1, F); printf("changed\n"); 
	} 

	printf("update your last name (%s) ?", user->l_name); scanf("%s", str);
	if (strcmp(str, "yes") == 0) {
		printf("Enter new last name (up to 10 characters) : ");
		get_str(user->l_name, 1, F_NAME - 1, F); printf("changed\n");
	}

	printf("update your phone number (%s) ?", user->phone); scanf("%s", str);
	if (strcmp(str, "yes") == 0) {
		printf("Enter new phone number (10 digits) : ");
		get_str(user->phone, PHONE-1, PHONE - 1, T); printf("changed\n");
	}

	printf("update your age (%s) ?", user->age); scanf("%s", str);
	if (strcmp(str, "yes") == 0) {
		printf("Enter new age (16-70) : "); scanf("%s", user->age);
		while (checkAge(user->age)==F) { printf("invalid input ! try again \n"); scanf("%s", user->age); }
		printf("changed\n");
	}

	printf("\nDone !\n");
}

int select_filters(int *j_filter)
{
	int sub_filter = 0;
	printf("\n~ Job search \nSelect a number of job filtering category\n");
	printf("1. Profession \n2. Location \n3. Job type \n4. Experience \n5. View all\n");
	*j_filter = type_number(1, 5);
	switch (*j_filter)
	{
	case 1:
		printf("\nSelect profession : \n1. Software Engineer\n2. Doctor\n3. Teacher\n4. Butcher\n");
		sub_filter = type_number(1, 4);
		break;
	case 2:
		printf("\nSelect location : \n1. Tel Aviv\n2. Ashdod\n3. Beer Sheva\n4. Haifa\n5.Work from home\n");
		sub_filter = type_number(1, 5);
		break;
	case 3:
		printf("\nSelect job type : \n1. Half time\n2. Full time\n3. Freelancer\n");
		sub_filter = type_number(1, 3);
		break;
	case 4:
		printf("\nSelect experience : \n1. No experience needed\n2. 1 - 2 years\n3. 3+ years\n");
		sub_filter = type_number(1, 3);
		break;
	}
	return sub_filter;
}

int print_relevant_jobs(int j_filter, int sub_filter, struct job * jobs, int j_size, char * purpose)
{
	int index = INVALID;
	int flag = 0;
	char answer[MAX_STR];
	for (int i = 0; i < j_size; i++)
	{
		switch (j_filter)
		{
		case 1:
			if ((jobs[i].profession == sub_filter) && (jobs[i].total < 3)) { print_job(jobs+i, i); flag = 1; }
			break;
		case 2:
			if ((jobs[i].location == sub_filter) && (jobs[i].total < 3)) { print_job(jobs+i, i), i; flag = 1; }
			break;
		case 3:
			if ((jobs[i].type == sub_filter) && (jobs[i].total < 3)) { print_job(jobs+i, i); flag = 1; }
			break;
		case 4:
			if ((jobs[i].experience == sub_filter) && (jobs[i].total < 3)) { print_job(jobs+i, i); flag = 1; }
			break;
		case 5:
			if ((jobs[i].total < 3) && (jobs[i].employer != INVALID)) { print_job(jobs+i, i); flag = 1; }
			break;
		}
	}
	if (flag == 0) { printf("\nNo suitable jobs were found\n"); }
	else
	{
		if (strcmp(purpose, "1") == 0)
		{
			printf("\nIf you want to submit your profile to one of the jobs, type 'yes' : "); scanf("%s", answer);
			if (strcmp(answer, "yes") == 0) {
				printf("Type a job number : "); scanf("%d", &index);
			}
		}
	}
	return index;
}

void print_job(struct job * job, int index)
{

	printf("\n\nJob number : %d \nProfession : ", index);
	switch (job->profession) {
	case 1:
		printf("Software Engineer\n"); break;
	case 2:
		printf("Doctor\n"); break;
	case 3:
		printf("Teacher\n"); break;
	case 4:
		printf("Butcher\n"); break;
	}
	printf("Location : ");
	switch (job->location)
	{
	case 1:
		printf("Tel Aviv\n"); break;
	case 2:
		printf("Ashdod\n"); break;
	case 3:
		printf("Beer Sheva\n"); break;
	case 4:
		printf("Haifa\n"); break;
	case 5:
		printf("Work from home\n"); break;
	}
	printf("Job type : ");
	switch (job->type)
	{
	case 1:
		printf("Half time\n"); break;
	case 2:
		printf("Full time\n"); break;
	case 3:
		printf("Freelancer\n"); break;
	}
	printf("Experience : ");
	switch (job->experience)
	{
	case 1:
		printf("No experience needed\n"); break;
	case 2:
		printf("1 - 2 years\n"); break;
	case 3:
		printf("3+ years\n"); break;
	}
	printf("\n");
}

boolean submit_profile(int index, int j_filter, int sub_filter, struct job * job)
{
	switch (j_filter)
	{
	case 1:
		if ((job->profession == sub_filter) && (job->total < 3)) {
			add_profile(index, job); return T;
		}
		return F;
	case 2:
		if ((job->location == sub_filter) && (job->total < 3)) {
			add_profile(index, job); return T;
		}
		return F;
	case 3:
		if ((job->type == sub_filter) && (job->total < 3)) {
			add_profile(index, job); return T;
		}
		return F;
	case 4:
		if ((job->experience == sub_filter) && (job->total < 3)) {
			add_profile(index, job); return T;
		}
		return F;
	case 5:
		if (job->total < 3) {
			add_profile(index, job); return T;
		}
		return F;
	}
	return F;
}

void add_profile(int index, struct job * job)
{
	printf("\nProfile successfully submitted! Back to the main menu\n");
	if (job->total == 0) { job->candidate1 = index; job->total++; return; }
	if (job->total == 1) { job->candidate2 = index; job->total++; return; }
	if (job->total == 2) { job->candidate3 = index; job->total++; }
	return;
}

void aptitude_test()
{
	char answer[10];
	printf("\n~Aptitude test\nDo you like working with people ?\n");
	scanf("%s", answer);
	if (strcmp(answer, "yes") == 0)
	{
		printf("Are you afraid of blood ?\n");
		scanf("%s", answer);
		if (strcmp(answer, "yes") == 0)
		{
			printf("you will be a good teacher ! \n");
		}
		else
		{
			printf("you will be a good doctor ! \n");

		}
	}
	else
	{
		printf("Are you afraid of blood ?\n");
		scanf("%s", answer);
		if (strcmp(answer, "yes") == 0)
		{
			printf("you will be a good software engineer !\n");
		}
		else
		{
			printf("you will be a good butcher !\n");
		}

	}
}

void publish_job(struct job * job, int index)
{
	job->employer = index;
	printf("\n~ Publish NewJob \n\nProfession : 1.Software Engineer 2.Doctor 3.Teacher 4.Butcher \n");
	job->profession = type_number(1, 4);

	printf("\nLocation: 1.Tel Aviv 2.Ashdod 3.Beer Sheva 4.Haifa 5.Work from home \n");
	job->location = type_number(1, 5);

	printf("\nJob type: 1.Half time 2.Full time 3.Freelancer \n");
	job->type = type_number(1, 3);

	printf("\nExperience: 1.No experience needed 2. 1 - 2 years 3. 3 + years\n");
	job->experience = type_number(1, 3);

	job->candidate1 = INVALID; job->candidate2 = INVALID; job->candidate3 = INVALID;
	job->total = 0;
	printf("\nJob posted successfully!\n");
}

void submissions_history(int index, struct job * jobs, int j_size)
{
	int flag = 0;
	for (int i = 0; i < j_size; i++)
	{
		if ((jobs[i].candidate1 == index) || (jobs[i].candidate3 == index) || (jobs[i].candidate3 == index)) {
			print_job(jobs + i, i);
			flag = 1;
		}
	}
	if (flag == 0) { printf("\nNo submissions were found !\n"); }
}

int view_all_my_jobs(int index, struct job ** jobs, int j_size)
{
	char ans[MAX_STR];
	int job_num;
	int flag = 0;
	for (int i = 0; i < j_size; i++)
	{
		if ((*jobs)[i].employer == index) {
			print_job((*jobs) + i, i);
			flag = 1;
		}
	}
	if (flag == 0)
	{
		printf("\nNo suitable jobs were found\n");
		return INVALID;
	}

	printf("\nIf you want to update a job, type yes "); scanf("%s", ans);
	if (strcmp(ans, "yes") == 0) {
		printf("\nEnter job number "); scanf("%d", &job_num);
		if ((job_num < 0) || (job_num >= j_size) || ((*jobs)[job_num].employer != index))
			printf("invalid job number ! \n");
		else {
			update_job((*jobs) + job_num);
			printf("\nJob updated successfully!\n");
		}
	}
	printf("\nIf you want to delete a job, type yes "); scanf("%s", ans);
	if (strcmp(ans, "yes") == 0) {
		printf("Enter job number "); scanf("%d", &job_num);
		if ((job_num < 0) || (job_num >= j_size) || ((*jobs)[job_num].employer != index))
			printf("invalid job number ! \n");
		else {
			delete_job((*jobs) + job_num);
			printf("Job deleted successfully!\n");
		}
	}
	printf("\nIf you want to view the profiles of candidates for a particular job, type yes "); scanf("%s", ans);
	if (strcmp(ans, "yes") == 0) {
		printf("\nEnter job number "); scanf("%d", &job_num);
		if ((job_num < 0) || (job_num >= j_size) || ((*jobs)[job_num].employer != index))
			printf("invalid job number ! \n");
		else
			return job_num;
	}
	return INVALID;
}

void update_job(struct job *job)
{
	char answer[10];
	printf("\n~Update a job\nType 'yes' for a field you want to update\n");
	printf("Update profession ? "); scanf("%s", answer);
	if (strcmp(answer, "yes") == 0) {
		printf("select new profession: 1.Software Engineer 2.Doctor 3.Teacher 4.Butcher");
		job->profession = type_number(1, 4);
	}

	printf("Update location ? "); scanf("%s", answer);
	if (strcmp(answer, "yes") == 0) {
		printf("select new location: 1.Tel Aviv 2.Ashdod 3.Beer Sheva 4.Haifa 5.Work from home ");
		job->location = type_number(1, 5);
	}

	printf("Update job type ? "); scanf("%s", answer);
	if (strcmp(answer, "yes") == 0) {
		printf("select new job type: 1.Half time 2.Full time 3.Freelancer ");
		job->type = type_number(1, 3);
	}

	printf("Update experience ? "); scanf("%s", answer);
	if (strcmp(answer, "yes") == 0) {
		printf("select new experience: 1. No experience needed 2. 1 - 2 years 3. 3 + years");
		job->experience = type_number(1, 3);
	}

	job->candidate1 = INVALID; job->candidate2 = INVALID; job->candidate3 = INVALID; job->total = 0;
}

void delete_job(struct job * job)
{
	job->employer = INVALID;
	job->profession = INVALID;
	job->location = INVALID;
	job->type = INVALID;
	job->experience = INVALID;
	job->candidate1 = INVALID;
	job->candidate2 = INVALID;
	job->candidate3 = INVALID;
	job->total = INVALID;
}

void print_user(struct user c)
{
	printf("\nfirst name : %s \n", c.f_name);
	printf("last name : %s \n", c.l_name);
	printf("ID number : %s \n", c.id);
	printf("Age : %s \n", c.age);
	printf("Phone number : %s \n", c.phone);
}


// check input
int type_number(int start, int end)
{
	int select;
	printf("Please enter number between %d - %d ", start, end); scanf("%d", &select);
	while ((select < start) || (select > end))
	{
		printf("invalid input ! type number between %d - %d ", start, end); scanf("%d", &select);
	}
	return select;
}

boolean isNumber(const char* s)
{
	for (int i = 0; s[i] != '\0'; i++)
	{
		if (isdigit(s[i]) == 0)
			return F;
	}
	return T;
}

int isdigit(char c)
{
	if ((c < '0') || (c > '9'))
		return 0;
	return 1;
}

boolean checkAge(char* age)
{
	int value;
	value = atoi(age);
	if (value < 16 || value>70)
		return F;
	return T;

}

void get_str(char * str, size_t start, size_t end, boolean only_numbers)
{
	scanf("%s", str);
	if (only_numbers == T) {
		while ((strlen(str) < start) || (strlen(str) > end) || (isNumber(str) == F))
		{
			printf("invalid input ! try again \n"); scanf("%s", str);
		}
		return;
	}
	while ((strlen(str) < start) || (strlen(str) > end))
	{
		printf("invalid input ! try again \n"); scanf("%s", str);
	}
}

boolean id_exist(char * id, struct login * login, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (strcmp(id, login[i].id) == 0)
			return T;
	}
	return F;
}


//write to files - end 

void write_to_size(int size)
{
	FILE* sz = NULL;
	sz = fopen("sz1.txt", "w");  if (!sz) { printf("ERROR ! "); return; }
	fprintf(sz, "%d", size);
	fclose(sz);
	return;
}

void write_to_users(struct user* users, int size)
{
	FILE* usr = NULL;
	usr = fopen("users1.txt", "w"); if (!usr) { return; } // להחזיר שגיאה
	for (int i = 0; i < size; i++)
	{
		fprintf(usr, "%s %s ", users[i].f_name, users[i].l_name);
		fprintf(usr, "%s %s ", users[i].id, users[i].phone);
		fprintf(usr, "%s %s ", users[i].age, users[i].pass);
		fprintf(usr, "%s\n", users[i].purpose);
	}
	fclose(usr);
	return;
}

void write_to_login(struct login* login, int size)
{
	FILE* lg = NULL;
	lg = fopen("login1.txt", "w"); if (!lg) { return; } // להחזיר שגיאה
	for (int i = 0; i < size; i++)
	{
		fprintf(lg, "%s %s\n", login[i].id, login[i].pass);
	}
	fclose(lg);
	return;
}

void write_to_jsize(int size)
{
	FILE* sz = NULL;
	sz = fopen("jsz1.txt", "w");  if (!sz) { printf("ERROR ! "); return; }
	fprintf(sz, "%d", size);
	fclose(sz);
	return;
}

void write_to_jobs(struct job* jobs, int size)
{
	FILE* jb = NULL;
	jb = fopen("jobs1.txt", "w"); if (!jb) { return; } // להחזיר שגיאה
	for (int i = 0; i < size; i++)
	{
		fprintf(jb, "%d %d ", jobs[i].employer, jobs[i].profession);
		fprintf(jb, "%d %d ", jobs[i].location, jobs[i].type);
		fprintf(jb, "%d %d ", jobs[i].experience, jobs[i].candidate1);
		fprintf(jb, "%d %d %d\n", jobs[i].candidate2, jobs[i].candidate3, jobs[i].total);
	}
	fclose(jb);
	return;
}
