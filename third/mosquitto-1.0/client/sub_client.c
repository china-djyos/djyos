/*
Copyright (c) 2009-2012 Roger Light <roger@atchoo.org>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of mosquitto nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <unistd.h>
#else
#include <process.h>
#include <winsock2.h>
#define snprintf sprintf_s
#endif

#include <mosquitto.h>

/* This struct is used to pass data to callbacks.
 * An instance "ud" is created in main() and populated, then passed to
 * mosquitto_new(). */
struct userdata {
	char **topics;
	int topic_count;
	int topic_qos;
	char *username;
	char *password;
	int verbose;
	bool quiet;
};

static void my_message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
	struct userdata *ud;

	assert(obj);
	ud = (struct userdata *)obj;

	if(ud->verbose){
		if(message->payloadlen){
			printf("%s %s\n\r", message->topic, (const char *)message->payload);
		}else{
			printf("%s (null)\n\r", message->topic);
		}
		fflush(stdout);
	}else{
		if(message->payloadlen){
			printf("%s\n\r", (const char *)message->payload);
			fflush(stdout);
		}
	}
}

static void my_connect_callback(struct mosquitto *mosq, void *obj, int result)
{
	int i;
	struct userdata *ud;

	assert(obj);
	ud = (struct userdata *)obj;

	if(!result){
		for(i=0; i<ud->topic_count; i++){
			mosquitto_subscribe(mosq, NULL, ud->topics[i], ud->topic_qos);
		}
	}else{
		if(result && !ud->quiet){
			fprintf(stderr, "%s\n\r", mosquitto_connack_string(result));
		}
	}
}

static void my_subscribe_callback(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
	int i;
	struct userdata *ud;

	assert(obj);
	ud = (struct userdata *)obj;

	if(!ud->quiet) printf("Subscribed (mid: %d): %d", mid, granted_qos[0]);
	for(i=1; i<qos_count; i++){
		if(!ud->quiet) printf(", %d", granted_qos[i]);
	}
	if(!ud->quiet) printf("\n\r");
}

static void my_log_callback(struct mosquitto *mosq, void *obj, int level, const char *str)
{
	printf("%s\n\r", str);
}

static void print_usage(void)
{
	printf("mosquitto_sub is a simple mqtt client that will subscribe to a single topic and print all messages it receives.\n\r\n\r");
	printf("Usage: mosquitto_sub [-c] [-h host] [-k keepalive] [-p port] [-q qos] [-v] -t topic ...\n\r");
	printf("                     [-i id] [-I id_prefix]\n\r");
	printf("                     [-d] [--quiet]\n\r");
	printf("                     [-u username [-P password]]\n\r");
	printf("                     [--will-topic [--will-payload payload] [--will-qos qos] [--will-retain]]\n\r");
	printf("                     [{--cafile file | --capath dir} [--cert file] [--key file]]\n\r");
	printf("                     [--psk hex-key --psk-identity identity]\n\r");
	printf("       mosquitto_sub --help\n\r\n\r");
	printf(" -c : disable 'clean session' (store subscription and pending messages when client disconnects).\n\r");
	printf(" -d : enable debug messages.\n\r");
	printf(" -h : mqtt host to connect to. Defaults to localhost.\n\r");
	printf(" -i : id to use for this client. Defaults to mosquitto_sub_ appended with the process id.\n\r");
	printf(" -I : define the client id as id_prefix appended with the process id. Useful for when the\n\r");
	printf("      broker is using the clientid_prefixes option.\n\r");
	printf(" -k : keep alive in seconds for this client. Defaults to 60.\n\r");
	printf(" -p : network port to connect to. Defaults to 1883.\n\r");
	printf(" -q : quality of service level to use for the subscription. Defaults to 0.\n\r");
	printf(" -t : mqtt topic to subscribe to. May be repeated multiple times.\n\r");
	printf(" -u : provide a username (requires MQTT 3.1 broker)\n\r");
	printf(" -v : print published messages verbosely.\n\r");
	printf(" -P : provide a password (requires MQTT 3.1 broker)\n\r");
	printf(" --help : display this message.\n\r");
	printf(" --quiet : don't print error messages.\n\r");
	printf(" --will-payload : payload for the client Will, which is sent by the broker in case of\n\r");
	printf("                  unexpected disconnection. If not given and will-topic is set, a zero\n\r");
	printf("                  length message will be sent.\n\r");
	printf(" --will-qos : QoS level for the client Will.\n\r");
	printf(" --will-retain : if given, make the client Will retained.\n\r");
	printf(" --will-topic : the topic on which to publish the client Will.\n\r");
	printf(" --cafile : path to a file containing trusted CA certificates to enable encrypted\n\r");
	printf("            certificate based communication.\n\r");
	printf(" --capath : path to a directory containing trusted CA certificates to enable encrypted\n\r");
	printf("            communication.\n\r");
	printf(" --cert : client certificate for authentication, if required by server.\n\r");
	printf(" --key : client private key for authentication, if required by server.\n\r");
	printf(" --psk : pre-shared-key in hexadecimal (no leading 0x) to enable TLS-PSK mode.\n\r");
	printf(" --psk-identity : client identity string for TLS-PSK mode.\n\r");
	printf("\n\rSee http://mosquitto.org/ for more information.\n\r\n\r");
}

int Submain(int argc, char *argv[])
{
	char *id = NULL;
	char *id_prefix = NULL;
	int i;
	char *host = "localhost";
	int port = 1883;
	int keepalive = 60;
	bool clean_session = true;
	bool debug = false;
	struct mosquitto *mosq = NULL;
	int rc;
	char hostname[256];
	char err[1024];
	struct userdata ud;
	int len;
	
	char *will_payload = NULL;
	long will_payloadlen = 0;
	int will_qos = 0;
	bool will_retain = false;
	char *will_topic = NULL;

	char *cafile = NULL;
	char *capath = NULL;
	char *certfile = NULL;
	char *keyfile = NULL;

	char *psk = NULL;
	char *psk_identity = NULL;

	memset(&ud, 0, sizeof(struct userdata));

	for(i=1; i<argc; i++){
		if(!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port")){
			if(i==argc-1){
				fprintf(stderr, "Error: -p argument given but no port specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				port = atoi(argv[i+1]);
				if(port<1 || port>65535){
					fprintf(stderr, "Error: Invalid port given: %d\n\r", port);
					print_usage();
					return 1;
				}
			}
			i++;
		}else if(!strcmp(argv[i], "-c") || !strcmp(argv[i], "--disable-clean-session")){
			clean_session = false;
		}else if(!strcmp(argv[i], "--cafile")){
			if(i==argc-1){
				fprintf(stderr, "Error: --cafile argument given but no file specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				cafile = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "--capath")){
			if(i==argc-1){
				fprintf(stderr, "Error: --capath argument given but no directory specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				capath = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "--cert")){
			if(i==argc-1){
				fprintf(stderr, "Error: --cert argument given but no file specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				certfile = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")){
			debug = true;
		}else if(!strcmp(argv[i], "--help")){
			print_usage();
			return 0;
		}else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--host")){
			if(i==argc-1){
				fprintf(stderr, "Error: -h argument given but no host specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				host = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "-i") || !strcmp(argv[i], "--id")){
			if(id_prefix){
				fprintf(stderr, "Error: -i and -I argument cannot be used together.\n\r\n\r");
				print_usage();
				return 1;
			}
			if(i==argc-1){
				fprintf(stderr, "Error: -i argument given but no id specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				id = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "-I") || !strcmp(argv[i], "--id-prefix")){
			if(id){
				fprintf(stderr, "Error: -i and -I argument cannot be used together.\n\r\n\r");
				print_usage();
				return 1;
			}
			if(i==argc-1){
				fprintf(stderr, "Error: -I argument given but no id prefix specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				id_prefix = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "-k") || !strcmp(argv[i], "--keepalive")){
			if(i==argc-1){
				fprintf(stderr, "Error: -k argument given but no keepalive specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				keepalive = atoi(argv[i+1]);
				if(keepalive>65535){
					fprintf(stderr, "Error: Invalid keepalive given: %d\n\r", keepalive);
					print_usage();
					return 1;
				}
			}
			i++;
		}else if(!strcmp(argv[i], "--key")){
			if(i==argc-1){
				fprintf(stderr, "Error: --key argument given but no file specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				keyfile = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "--psk")){
			if(i==argc-1){
				fprintf(stderr, "Error: --psk argument given but no key specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				psk = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "--psk-identity")){
			if(i==argc-1){
				fprintf(stderr, "Error: --psk-identity argument given but no identity specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				psk_identity = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "-q") || !strcmp(argv[i], "--qos")){
			if(i==argc-1){
				fprintf(stderr, "Error: -q argument given but no QoS specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				ud.topic_qos = atoi(argv[i+1]);
				if(ud.topic_qos<0 || ud.topic_qos>2){
					fprintf(stderr, "Error: Invalid QoS given: %d\n\r", ud.topic_qos);
					print_usage();
					return 1;
				}
			}
			i++;
		}else if(!strcmp(argv[i], "--quiet")){
			ud.quiet = true;
		}else if(!strcmp(argv[i], "-t") || !strcmp(argv[i], "--topic")){
			if(i==argc-1){
				fprintf(stderr, "Error: -t argument given but no topic specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				ud.topic_count++;
				ud.topics = realloc(ud.topics, ud.topic_count*sizeof(char *));
				ud.topics[ud.topic_count-1] = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "-u") || !strcmp(argv[i], "--username")){
			if(i==argc-1){
				fprintf(stderr, "Error: -u argument given but no username specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				ud.username = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose")){
			ud.verbose = 1;
		}else if(!strcmp(argv[i], "-P") || !strcmp(argv[i], "--pw")){
			if(i==argc-1){
				fprintf(stderr, "Error: -P argument given but no password specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				ud.password = argv[i+1];
			}
			i++;
		}else if(!strcmp(argv[i], "--will-payload")){
			if(i==argc-1){
				fprintf(stderr, "Error: --will-payload argument given but no will payload specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				will_payload = argv[i+1];
				will_payloadlen = strlen(will_payload);
			}
			i++;
		}else if(!strcmp(argv[i], "--will-qos")){
			if(i==argc-1){
				fprintf(stderr, "Error: --will-qos argument given but no will QoS specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				will_qos = atoi(argv[i+1]);
				if(will_qos < 0 || will_qos > 2){
					fprintf(stderr, "Error: Invalid will QoS %d.\n\r\n\r", will_qos);
					return 1;
				}
			}
			i++;
		}else if(!strcmp(argv[i], "--will-retain")){
			will_retain = true;
		}else if(!strcmp(argv[i], "--will-topic")){
			if(i==argc-1){
				fprintf(stderr, "Error: --will-topic argument given but no will topic specified.\n\r\n\r");
				print_usage();
				return 1;
			}else{
				will_topic = argv[i+1];
			}
			i++;
		}else{
			fprintf(stderr, "Error: Unknown option '%s'.\n\r",argv[i]);
			print_usage();
			return 1;
		}
	}

	if(clean_session == false && (id_prefix || !id)){
		if(!ud.quiet) fprintf(stderr, "Error: You must provide a client id if you are using the -c option.\n\r");
		return 1;
	}

	if(ud.topic_count == 0){
		fprintf(stderr, "Error: You must specify a topic to subscribe to.\n\r");
		print_usage();
		return 1;
	}
	if(will_payload && !will_topic){
		fprintf(stderr, "Error: Will payload given, but no will topic given.\n\r");
		print_usage();
		return 1;
	}
	if(will_retain && !will_topic){
		fprintf(stderr, "Error: Will retain given, but no will topic given.\n\r");
		print_usage();
		return 1;
	}
	if(ud.password && !ud.username){
		if(!ud.quiet) fprintf(stderr, "Warning: Not using password since username not set.\n\r");
	}
	if((certfile && !keyfile) || (keyfile && !certfile)){
		fprintf(stderr, "Error: Both certfile and keyfile must be provided if one of them is.\n\r");
		print_usage();
		return 1;
	}
	if((cafile || capath) && psk){
		if(!ud.quiet) fprintf(stderr, "Error: Only one of --psk or --cafile/--capath may be used at once.\n\r");
		return 1;
	}
	if(psk && !psk_identity){
		if(!ud.quiet) fprintf(stderr, "Error: --psk-identity required if --psk used.\n\r");
		return 1;
	}

	mosquitto_lib_init();

	if(id_prefix){
		id = malloc(strlen(id_prefix)+10);
		if(!id){
			if(!ud.quiet) fprintf(stderr, "Error: Out of memory.\n\r");
			mosquitto_lib_cleanup();
			return 1;
		}
		snprintf(id, strlen(id_prefix)+10, "%s%d", id_prefix, getpid());
	}else if(!id){
		hostname[0] = '\0';
		gethostname(hostname, 256);
		hostname[255] = '\0';
		len = strlen("mosqsub/-") + 6 + strlen(hostname);
		id = malloc(len);
		if(!id){
			if(!ud.quiet) fprintf(stderr, "Error: Out of memory.\n\r");
			mosquitto_lib_cleanup();
			return 1;
		}
		snprintf(id, len, "mosqsub/%d-%s", getpid(), hostname);
		id[MOSQ_MQTT_ID_MAX_LENGTH] = '\0';
	}

	mosq = mosquitto_new(id, clean_session, &ud);
	if(!mosq){
		switch(errno){
			case ENOMEM:
				if(!ud.quiet) fprintf(stderr, "Error: Out of memory.\n\r");
				break;
			case EINVAL:
				if(!ud.quiet) fprintf(stderr, "Error: Invalid id and/or clean_session.\n\r");
				break;
		}
		mosquitto_lib_cleanup();
		return 1;
	}
	if(debug){
		mosquitto_log_callback_set(mosq, my_log_callback);
	}
	if(will_topic && mosquitto_will_set(mosq, will_topic, will_payloadlen, will_payload, will_qos, will_retain)){
		if(!ud.quiet) fprintf(stderr, "Error: Problem setting will.\n\r");
		mosquitto_lib_cleanup();
		return 1;
	}
	if(ud.username && mosquitto_username_pw_set(mosq, ud.username, ud.password)){
		if(!ud.quiet) fprintf(stderr, "Error: Problem setting username and password.\n\r");
		mosquitto_lib_cleanup();
		return 1;
	}
	if((cafile || capath) && mosquitto_tls_set(mosq, cafile, capath, certfile, keyfile, NULL)){
		if(!ud.quiet) fprintf(stderr, "Error: Problem setting TLS options.\n\r");
		mosquitto_lib_cleanup();
		return 1;
	}
	if(psk && mosquitto_tls_psk_set(mosq, psk, psk_identity, NULL)){
		if(!ud.quiet) fprintf(stderr, "Error: Problem setting TLS-PSK options.\n\r");
		mosquitto_lib_cleanup();
		return 1;
	}
	mosquitto_connect_callback_set(mosq, my_connect_callback);
	mosquitto_message_callback_set(mosq, my_message_callback);
	if(debug){
		mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);
	}

	rc = mosquitto_connect(mosq, host, port, keepalive);
	if(rc){
		if(!ud.quiet){
			if(rc == MOSQ_ERR_ERRNO){
#ifndef WIN32
				strerror_r(errno, err, 1024);
#else
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errno, 0, (LPTSTR)&err, 1024, NULL);
#endif
				fprintf(stderr, "Error: %s\n\r", err);
			}else{
				fprintf(stderr, "Unable to connect (%d).\n\r", rc);
			}
		}
		return rc;
		mosquitto_lib_cleanup();
	}

	do{
		rc = mosquitto_loop(mosq, -1, 100);
	}while(rc == MOSQ_ERR_SUCCESS);

	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	if(rc){
		if(rc == MOSQ_ERR_ERRNO){
			fprintf(stderr, "Error: %s\n\r", strerror(errno));
		}else{
			fprintf(stderr, "Error: %s\n\r", mosquitto_strerror(rc));
		}
	}
	return rc;
}


