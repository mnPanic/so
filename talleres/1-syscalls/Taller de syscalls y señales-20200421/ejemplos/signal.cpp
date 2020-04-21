#include<stdio.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
using namespace std;

void signal3_handler1(int sig){
	cout<<"1: Recibí la señal 3, soy el proceso 1"<<endl;
	return;
}

void signal3_handler2(int sig){
	cout<<"2: Recibí la señal 3, soy el proceso 2"<<endl;
	return;
}


void hijo1(){
	signal(3,signal3_handler1);
	cout<<"1: Creado, durmiendo 10 segundos..."<<endl;
	int q=sleep(10);
	cout<<"1: Termino mi ejecución faltando "<<q<<" segundos de sueño"<<endl;
	exit(EXIT_SUCCESS);
}

void hijo2(){
	signal(3,signal3_handler2);
	cout<<"2: Creado, durmiendo 10 segundos..."<<endl;
	int q=sleep(10);
	cout<<"2: Termino mi ejecución faltando "<<q<<" segundos de sueño"<<endl;
	exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[])
{
	pid_t pid_h1;
	pid_t pid_h2;

	pid_h1 = fork();
	if(pid_h1 == 0)hijo1();

	pid_h2 = fork();
	if(pid_h2 == 0)hijo2();

	int numero;
	cin>>numero; 
	cout<<"envío la señal número "<< numero <<" al hijo 1"<<endl;
	kill(pid_h1,numero);
	cout<<"envío la señal número "<< numero <<" al hijo 2"<<endl;
	kill(pid_h2,numero);


	return 0;
}