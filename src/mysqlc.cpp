/*
	¸ÃÀà¹¦ÄÜÈçÏÂ:
		ÊµÏÖ²éÑ¯,²åÈëµÈMYSQL²Ù×÷,·µ»Ø½á¹û

	·µ»Ø½á¹û»ñÈ¡·½Ê½:  Í¨¹ım_dataÖ¸ÕëÉêÇëÏàÓ¦µÄÄÚ´æ,Ã¿¸ö×Ö¶Î¸ù¾İMYSQL×Ö·û±ãÀË
	    ×î´ó255×Ö½ÚµÄÉè¶¨,È¡256×Ö½Ú,¼´ËùÓĞ¼ÇÂ¼µÄÃ¿¸ö×Ö¶Î°´ÕÕ256×Ö½ÚµÄ»º³åÇøË³
		Ğò´æ·Å.
		Í¨¹ıgetfieldcount()¡¢getrecordcount()¡¢getvalue(int index)Èı¸öº¯Êı¿ÉÒÔ
		¶ÁÈ¡Êı¾İ»º³åÇøµÄÈÎÒâÒ»¸ö×Ö¶ÎµÄ×Ö·û´®ĞÎÊ½µÄÖµ

	±àÒëĞèÇó:
		°²×°yum install -y mysql-devel.x86_64
		±àÒëÊ±Ôö¼Ó  -l mysqlclient Ñ¡Ïî
		°üº¬ÎÄ¼şÄ¿Â¼  /usr/include/mysql/

        °²×° 
		yum install mariadb-devel.x86_64
		yum install -y libcurl-devel.x86_64
		cp /usr/lib64/mysql/libmysqlclient.so* /usr/lib64/
		ĞŞ¸ÄÊı¾İ¿âÃÜÂë 
		Ìí¼Óroot·ÃÎÊµÄÈ¨ÏŞ 



*/
#include "../header/mysqlc.h"

MY_SQLC::MY_SQLC()
{
	memset(&m_conn,0,sizeof(MYSQL));
	m_res = NULL;
	m_data = NULL;
	memset(&m_row,0,sizeof(MYSQL_ROW));
	memset(m_host,0,128);
	memset(m_sqlcom,0,4096);
	memset(m_username,0,128);
	memset(m_userpwd,0,128);
	memset(m_dbname,0,128);
	memset(m_msgbox,0,128);
}

MY_SQLC::~MY_SQLC()
{


}
/*
	º¯Êı:MYSQLÊı¾İ¿â·ÃÎÊÀà³õÊ¼»¯
	¹¦ÄÜ:
		³õÊ¼»¯²¢Á¬½ÓÊı¾İ¿â
		¸üĞÂÓÃ»§»òÊı¾İ¿âÎÄ¼şÃû£¬²»»»µØÖ·
		Èç¹ûÊı¾İ¿âÃ»ÓĞ¹Ø±Õ£¬·µ»Ø´íÎó
	²ÎÊı:
	·µ»Ø:
	ËµÃ÷:
	
	  ¼ÇµÃÒ»¶¨ÒªÈ·¶¨ÉÏ´ÎÊ¹ÓÃºóÒÑ¾­¹Ø±Õ
*/
int	MY_SQLC::init(char *host,char *username,char *userpwd,char *dbname){
	MYSQL *rcp = NULL;
	//int rc = 0;
	if(m_conn.server_status>0) 
	{
		sprintf(m_msgbox,"mysql handle is not emtye! close mysql!\n");
		return -1;
    }

	//³õÊ¼»¯Êı¾İ¿âÁ¬½Ó£¬¼ÇµÃ±ØĞë
	if(host != NULL) 
	{
		memset(&m_conn,0,sizeof(MYSQL));
		m_res = NULL;
		memset(&m_row,0,sizeof(MYSQL_ROW));
		memset(m_host,0,128);
		memset(m_sqlcom,0,128);
		memset(m_username,0,128);
		memset(m_userpwd,0,128);
		memset(m_dbname,0,128);
		memset(m_msgbox,0,128);

		//³õÊ¼»¯±äÁ¿
		strcpy(m_host,host);
		strcpy(m_username,username);
		strcpy(m_userpwd,userpwd);
		strcpy(m_dbname,dbname);
	}
	else  
	{
		strcpy(m_username,username);
		strcpy(m_userpwd,userpwd);
		strcpy(m_dbname,dbname);
	}

	//²ÎÊı¼ì²é
	if(m_host == NULL) return -1;

	//³õÊ¼»¯MYSQL½á¹¹
	rcp = mysql_init(&m_conn);
    if(rcp == NULL) {
		sprintf(m_msgbox,"error code:%d %s\n",mysql_errno(&m_conn),mysql_error(&m_conn));
		printf("msg = %s\n",m_msgbox);
		return -1;
    }
	
	//printf("mysql_init:m_host = %s  user = %s  dbname= %s pwd = %s\n",m_host,m_username,m_dbname,userpwd);	
	//Á¬½ÓÊı¾İ¿â
    rcp = mysql_real_connect(&m_conn,m_host,m_username,m_userpwd,m_dbname,0,NULL,0);
	//rc =(int) mysql_change_user(&g_conn,username,NULL,db);
 	//rc = mysql_select_db(&m_conn,"mysql");
    if(rcp == NULL) 
	{
		sprintf(m_msgbox,"open mysql database failed! error code:%d %s\n",(signed int)mysql_errno(&m_conn),mysql_error(&m_conn));
		//sprintf(m_msgbox,"open mysql database failed!\n");
		//printf("msg = %s\n",m_msgbox);
		return -1;
	}
	return 0;
}


/*
	º¯Êı:MYSQLÊı¾İ¿â·ÃÎÊÀà³õÊ¼»¯
	²ÎÊı:
	·µ»Ø:
	ËµÃ÷:
	
	  ¼ÇµÃÒ»¶¨ÒªÈ·¶¨ÉÏ´ÎÊ¹ÓÃºóÒÑ¾­¹Ø±Õ
*/
int MY_SQLC::query(char *sqlcom)
{
	int rc = 0;
	int i=0,j=0,k=0;
        memset(m_sqlcom,0,4096);
	strcpy(m_sqlcom,sqlcom);

	//²éÑ¯
	/*
	mysql_real_query±Èmysql_query¶àÁË¸ö²ÎÊı: ×Ö·û´®queryµÄ³¤¶È, ËùÒÔÊÊºÏÓĞ¶ş½øÖÆÊı¾İµÄquery, ¶ømysql_queryµÄ×Ö·û´®query²»ÄÜ°üº¬¶ş½øÖÆ,ÒòÎªËüÒÔ\0Îª½áÎ²
	*/
	rc = mysql_real_query(&m_conn,m_sqlcom,strlen(m_sqlcom));//ÊÊºÏ¶ş½øÖÆ´úÂë
	//rc = mysql_query(&m_conn,m_sqlcom);//ÊÊºÏ×Ö·û´®
	if(rc != 0) {
		sprintf(m_msgbox,"SQL:query error code:%d %s\n",mysql_errno(&m_conn),mysql_error(&m_conn));
		return -1;
	}
	//»ñÈ¡²éÑ¯½á¹û
#if 0
	/*
		mysql_use_resultº¯Êı²»½«¼ÇÂ¼ÄÚÈİµ½±¾µØÄÚ´æ£¬
		¼ÇÂ¼ÊıÁ¿£¬×Ö¶ÎĞÅÏ¢ÎŞ·¨Ö±½Ó´ÓÊı¾İ½á¹¹ÖĞ»ñÈ¡
		¼ÇÂ¼ĞèÒªÍ¨¹ımysql_fetch_rowº¯Êı´ÓÊı¾İ¿âÖĞ
		Öğ¸ö¶ÁÈ¡
	*/
	m_res = mysql_use_result(&m_conn); 
	//½«²éÑ¯½á¹û×ª»»³É×Ö·û´®Êı×é
	m_row = mysql_fetch_row(m_res);
	printf("row_count = %ld\n",(long int)mysql_num_rows(m_res));
	printf("MySQL Tables in mysql database:\n"); 
	while ((m_row = mysql_fetch_row(m_res)) != NULL) 
		printf("%03d: %20s %20s %20s %20s\n",mysql_row_tell(m_res),m_row[0],m_row[1],m_row[2],m_row[3]); 
#endif

	/*
		mysql_store_resultº¯ÊıÍêÈ«¶ÁÈ¡¼ÇÂ¼ÄÚÈİµ½±¾µØÄÚ´æÖĞ£¬
		¼ÇÂ¼ÊıÁ¿£¬×Ö¶ÎĞÅÏ¢¿ÉÒÔÖ±½Ó´ÓÊı¾İ½á¹¹ÖĞ»ñÈ¡
	*/
	m_res = mysql_store_result(&m_conn); 
	//»ñÈ¡×Ö¶ÎÊıÁ¿
	m_fcount = mysql_num_fields(m_res);//printf("field_count = %ld\n",m_fcount);
	//»ñÈ¡¼ÇÂ¼ÊıÁ¿
	m_rcount = mysql_num_rows(m_res);//printf("row_count = %lu\n",m_rcount);

	//ÉêÇëÊı¾İ±íÄÚ´æ,mysqlÃ¿¸ö×Ö·û´®±äÁ¿×î´óÎª255×Ö½Ú,Òò´ËÉêÇëµÄÃ¿¸ö×Ö·û´®±äÁ¿Îª256×Ö½Ú
	m_data = (char *)malloc(m_rcount * m_fcount * 256*2);	
	if(m_data == NULL) 
	{
		sprintf(m_msgbox,"SQL:query succeed but memory malloc failed!");
		return -1;
	}
#if 0
	for(rc =0;rc<m_fcount;rc++)
	{
		m_field = mysql_fetch_field(m_res);
		printf("%20s ",m_field->name);
	}
	printf("\n");
#endif

	for(i =0;i<m_rcount;i++)
	{
		m_row = mysql_fetch_row(m_res);
		for(j =0;j<m_fcount;j++){ 
                     if(m_row[j]==NULL)
                            sprintf(&m_data[256*k++],"%s","null"); 
                     else 
                            sprintf(&m_data[256*k++],"%s",m_row[j]);
                  	}
        }

	

	//mysql_autocommit(m_conn,1);//Ç¿ÖÆÁ¢¼´Ö´ĞĞÉÏÃæµÄÃüÁî
	//Êı¾İÁ¿ºÜ´óµÄ²éÑ¯²ÅĞèÒªÕâ¸ö
	mysql_free_result(m_res);

	
	return 0;
}

int	 MY_SQLC::query_count(char *sqlcom)
{
	int rc = 0;
        memset(m_sqlcom,0,4096);
	strcpy(m_sqlcom, sqlcom);




	rc = mysql_real_query(&m_conn, m_sqlcom, strlen(m_sqlcom));//¿¿¿¿¿¿¿
	if (rc != 0) {
		sprintf(m_msgbox, "SQL:query error code:%d %s\n", mysql_errno(&m_conn), mysql_error(&m_conn));
		return -1;
	}
	//m_res = mysql_use_result(&m_conn);
	m_res = mysql_store_result(&m_conn); 
	int num =(int)mysql_num_rows(m_res); 
        mysql_free_result(m_res);
	return num;
}
										

int MY_SQLC::getfieldcount()
{
	return m_fcount;
}

int MY_SQLC::getrecordcount()
{
	return m_rcount;
}

int MY_SQLC::getvalue(char *value, int index)
{
	sprintf(value,"%s",&m_data[index*256]);
	return strlen(value);
}

int MY_SQLC::close()
{

	free(m_data);
	m_data = NULL;
    // close connection
    //mysql_free_result(m_res); 
	m_res = NULL;
    mysql_close(&m_conn); 

	return 0;
}

int	MY_SQLC::insert(char *sqlcom)
{
	int rc = 0;
	int i=0,j=0,k=0;

	strcpy(m_sqlcom,sqlcom);

	//²éÑ¯
	/*
	mysql_real_query±Èmysql_query¶àÁË¸ö²ÎÊı: ×Ö·û´®queryµÄ³¤¶È, ËùÒÔÊÊºÏÓĞ¶ş½øÖÆÊı¾İµÄquery, ¶ømysql_queryµÄ×Ö·û´®query²»ÄÜ°üº¬¶ş½øÖÆ,ÒòÎªËüÒÔ\0Îª½áÎ²
	*/
	rc = mysql_real_query(&m_conn,m_sqlcom,strlen(m_sqlcom));//ÊÊºÏ¶ş½øÖÆ´úÂë
	//rc = mysql_query(&m_conn,m_sqlcom);//ÊÊºÏ×Ö·û´®
	if(rc != 0) {
		sprintf(m_msgbox,"SQL:query error code:%d %s\n",mysql_errno(&m_conn),mysql_error(&m_conn));
		return -1;
	}
	return 0;
}

char *MY_SQLC::getmsg()
{
	return m_msgbox;
}
