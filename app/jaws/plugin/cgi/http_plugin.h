#ifndef	__HTTP_PLUGIN_INCLUDE_H__
#define	__HTTP_PLUGIN_INCLUDE_H__

#include "lib_acl.h"
#include "lib_protocol.h"

#ifdef	__cplusplus
extern "C" {
#endif

/**
 * HTTP ��ʼ��ע�ắ��, �ú�������ǿգ������������ڸտ�ʼ����ʱ����
 * @param dll_env {ACL_DLL_ENV*} DLL ��������, dll_env->logfp ��־�ļ����,
 *  �����ϣ������־������������־�ļ��У�����Խ���ֵ�ÿ�
 * @param dmptr {ACL_DEBUG_MEM*} �����ڴ���Եľ��
 * @param cfg_dir {const char*} �����ļ�����·������·���´�����ж�̬��������ļ�
 */
void http_plugin_init(ACL_DLL_ENV *dll_env, const char *cfg_dir);

/**
 * HTTP �������ע�ắ�����ú�������ǿգ��������������յ�HTTP����ͷʱ����
 * @param client {ACL_VSTREAM*} �ͻ�����
 * @param hdr_req {HTTP_HDR_REQ*} HTTP����ͷ
 * @param ctx_ptr {void**} ��ָ����������洢�û��Ķ�̬����
 * @return {int} < 0: ��ʾ��ֹ��HTTP����, ��ֵ��ʾ����ţ�����Ϊ��-5xx/-4xx;
 *  0: ��ʾ��HTTP�����������������; > 0: ��ʾ��HTTP������������ģ��ĵ���
 *  �̴߳���
 */
int http_request_filter(ACL_VSTREAM *client, HTTP_HDR_REQ *hdr_req, void **ctx_ptr);

/**
 * HTTP ����ӹܴ���ע�ắ�����ú�������ǿգ��� http_request_filter() > 0������
 * ��������ô˺�����ȫ�ӹܸ�HTTP������Ӧ���������ٴ�����HTTP����HTTP��Ӧ
 * @param client {ACL_VSTREAM*} HTTP�ͻ�������������
 * @param hdr_req {HTTP_HDR_REQ*} �ͻ���HTTP����ͷ
 * @param ctx {void*} �������Ĳ������ö�̬�������� http_request_filter �е� ctx_ptr
 *  �������ص�
 */
void http_request_forward(ACL_VSTREAM *client, HTTP_HDR_REQ *hdr_req, void *ctx);

/*----------------------------------------------------------------------------*/

#define	DEBUG_BASE	500
#define	DBG_REQ		(DEBUG_BASE + 1)
#define	DBG_RES		(DEBUG_BASE + 2)

#define	STR	acl_vstring_str
#define	LEN	ACL_VSTRING_LEN

typedef struct {
	char *alias;
	char *cgi;
	HTTP_HDR_REQ *hdr_req;
	HTTP_REQ *req;
	HTTP_HDR_RES *hdr_res;
	ACL_VSTREAM *client;
} CGI;

/* ȫ�����ñ��� */

extern char *var_cfg_log_name;
extern char *var_cfg_cgi_bin;
extern int   var_cfg_data_clone;

/* in http_conf.c */

/**
 * ���������ļ�
 * @param cfg_dir {const char*} �����ļ����ڵ�Ŀ¼λ��
 */
void http_conf_load(const char *cfg_dir);

/* in http_plugin.c */

/**
 * �����������̳߳�
 * @param threads_limit {int} �̳߳�������̸߳���
 * @param threads_idle {int} �̳߳���ÿ���̵߳�������ʱ��(��)
 */
void http_plugin_pool_create(int threads_limit, int threads_idle);

/**
 * ���̳߳������������û����ӵ��������̳߳��е�ĳ���߳̽ӹ�
 * @param start_routine {void (*)(void*)} �û����ӵ�����ص�����
 * @param arg {void*} �̳߳��е�ĳ���̻߳ص� start_routine ʱ�Ļص�����
 */
void http_plugin_pool_append(void (*start_routine)(void *), void *arg);

/**
 * ����Ҫ�����ڴ����״̬ʱ�ĺ���
 * @param level {int} ���Լ��𣬿��õļ���Ϊ: 1, 2, 3
 */
void http_plugin_debug_memory(int level);

/* in http_cgi.c */

void http_cgi_init(void);
void http_cgi_end(void);
void http_cgi_add2(const char *alias, const char *cgi);
void http_cgi_add1(const ACL_ARGV *args);
CGI *http_cgi_path(const char *alias);
void http_cgi_thread(void *ctx);

#ifdef	__cplusplus
}
#endif

#endif