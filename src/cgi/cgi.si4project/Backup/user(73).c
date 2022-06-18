#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sqlite3.h>

void do_insert(sqlite3 *db)
{
	int no;
	char name[16];
	float score;
	char sqlstr[128], *errmsg;

	printf("input no : ");
	scanf("%d", &no);
	printf("input name : ");
	scanf("%s", name);
	printf("input score : ");
	scanf("%f", &score);
	sprintf(sqlstr, "insert into student values (%d, '%s', %.1f)", 
	no, name, score);
	#if __DEBUG
	printf("cmd:%s\n",sqlstr);
	#endif
	if (sqlite3_exec(db, sqlstr, NULL, NULL, &errmsg) != 0)
	{
		printf("error : %s\n", sqlite3_errmsg(db));
	}
	else
	{
		printf("insert is done\n");
	}
	printf("\n");

	return;
}

void do_delete(sqlite3 *db)
{
	char *errmsg;
	char sqlstr[128], expression[64];

	printf("input expression : ");
	scanf("%s", expression);//name='ma'
	sprintf(sqlstr, "delete from student where %s", expression);
#if __DEBUG
	printf("cmd:%s\n",sqlstr);
#endif
	if (sqlite3_exec(db, sqlstr, NULL, NULL, &errmsg) != 0)
	{
		printf("error : %s\n", sqlite3_errmsg(db));
	}
	else
	{
		printf("deletet is done\n");
	}
	printf("\n");

	return;
}
 
int callback(void *para, int f_num, char **f_val, char **f_name)
{
	int i;

	for (i=0; i<f_num; i++)
	{
		printf("%-8s", f_val[i]);
	}
	printf("\n");

	return 0;
}

void do_show(sqlite3 *db)
{
	char *errmsg;

	printf("no      name    score\n");

	if (sqlite3_exec(db, "select * from student", callback, NULL, &errmsg) != 0)
	{
		printf("error : %s\n", sqlite3_errmsg(db));
	}
	printf("\n");

	return;
}

 void do_show_sample(sqlite3 *db)
 {
 	char **result, *errmsg;
	int nrow, ncolumn, i, j, index;

	if (sqlite3_get_table(db, "select * from student", &result, &nrow, &ncolumn, &errmsg) != 0)
	{
		printf("error : %s\n", errmsg);
		sqlite3_free(errmsg);
	}
	
	index = ncolumn;

	for (i=0; i<nrow; i++)
	{
		for (j=0; j<ncolumn; j++)
		{
			printf("%-8s : %-8s\n", result[j], result[index]);
			
			 
			index++;
		}
		printf("************************\n");
	}
	sqlite3_free_table(result);

	return;
 }
 

int main()
{
	sqlite3 *db;
	int n;
	int rc = 0;
	char *errmsg;	
	char clean[64];
/* Create SQL statement */
	char sql[] ="CREATE TABLE IF NOT EXISTS STUDENT(" \
	  "NO INT PRIMARY KEY     NOT NULL," \
	  "NAME           TEXT    NOT NULL," \
	  "SCORE            INT     NOT NULL);";


	if (sqlite3_open("my.db", &db) < 0)
	{
		printf("fail to sqlite3_open : %s\n", sqlite3_errmsg(db));
		return -1;
	}
	

  /* Execute SQL statement */
	rc = sqlite3_exec(db, sql, NULL, 0, &errmsg);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", errmsg);
		sqlite3_free(errmsg);
	} else {
		fprintf(stdout, "Table created successfully\n");
	}

	while ( 1 )
	{
		printf("*********************************************\n");
		printf("1: insert record   \n2: delete record  \n3: show record  \n4: quit\n");
		printf("*********************************************\n");
		printf("please select : "); 
		
		if (scanf("%d", &n) != 1)
		{
			fgets(clean, 64, stdin);
			printf("\n");
			continue;
		}
		switch ( n )
		{
			case 1 :
				do_insert(db);
				break;
			case 2 :
				do_delete(db);
				break;
			case 3 :
				do_show_sample(db);
				break;
			case 4 :
				sqlite3_close(db);
				exit(0);
		}
	}
	return 0;
}
