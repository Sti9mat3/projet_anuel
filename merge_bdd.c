#include <string.h>
#include <mysql/mysql.h>
#include <stdio.h>

#define DB_HOST "localhost"
#define DB_USER "root"
#define DB_PASS "mysql"
#define DB_NAME "testdb"

int main(int argc, char **argv)
{ 
  MYSQL_STMT *stmt;  
  MYSQL *con = mysql_init(NULL);
   
  int param_count;
  const char* query;

  if (con == NULL) 
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      exit(1);
  }

  if (mysql_real_connect(con, DB_HOST, DB_USER, DB_PASS, 
          DB_NAME, 0, NULL, 0) == NULL) 
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      mysql_close(con);
      exit(1);
  }  
/*
  if (mysql_query(con, "USE testdb")) 
  {
      fprintf(stderr, "%s\n", mysql_error(con));
      mysql_close(con);
      exit(1);
  }*/
  // char* today = DATE_FORMAT(NOW(),'_%Y_%m_%d_%H_%i_%s');
  // char* folder = '/var/lib/mysql-files/';
  // char* file = 'test';
  // char* ext = '.csv';
  query = "SELECT * FROM testtable INTO OUTFILE '/var/lib/mysql-files/DATE_FORMAT(NOW()\'_\%Y_\%m\').csv' FIELDS ENCLOSED BY '\"' TERMINATED BY ';' ESCAPED BY '\"' LINES TERMINATED BY '\r\n';";   
  stmt = mysql_stmt_init(con);
  if(!stmt)
  {
      fprintf(stderr, " mysql_stmt_init(), out of memory\n");
      fprintf(stderr, "%s\n", mysql_error(con));
      mysql_close(con);
      exit(1);
  }
  
  if(mysql_stmt_prepare(stmt, query, strlen(query)))
  {
  	fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
  	fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
  	exit(0);
  }


  fprintf(stdout, " prepare, SELECT successful\n");

  param_count= mysql_stmt_param_count(stmt);
  fprintf(stdout, " total parameters in SELECT: %d\n", param_count);

  if (param_count != 0) /* validate parameter count */
  {
    fprintf(stderr, " invalid parameter count returned by MySQL\n");
    exit(0);
  }

  if (mysql_stmt_execute(stmt))
  {
  	fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
  	fprintf(stderr, " %s\n", mysql_stmt_error(stmt));
  	exit(0);
  }
	printf("la requete a été executé");





  mysql_stmt_close(stmt);
  mysql_close(con);
  exit(0);
}

