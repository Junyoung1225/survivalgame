#include <iostream>
#include <string.h>



void printfstat(int heart, int mental, int food, int water, int bullet) {
	printf("������������������������������������������������������\n");
	printf("�� ü�� :     ");
	for (int i = 0; i < heart; i++) {
		printf("��");
	}
	for(int i = 0; i < 5 - heart; i++) {
		printf("��");
	}
	printf("   ��\n");

	printf("�� ��Ʈ���� : ");
	for (int i = 0; i < mental; i++) {
		printf("��");
	}
	for (int i = 0; i < 5 - mental; i++) {
		printf("��");
	}
	printf("   ��\n");


	printf("��                         ��\n"); 
	printf("�� �ķ� X %d                ��\n",food); 
	printf("�� �� X %d                  ��\n",water);
	if (bullet != 0) {
		printf("�� źȯ X %d                ��\n", bullet);
	}
	printf("������������������������������������������������������\n");
}

int main() {
	
	printfstat(1, 2, 3, 4,5);
}