#include <time.h>
#include "battle.h"

int main(int argc, char** argv) {
	srand(time(NULL));
	
	Player p1("Samqo", 10, 10, 2, 0, 0, 20, 1.2);
	Enemy e1("Ciganek", 10, 10, 1, 0, 10, 3);
	
	Battle b1(&p1, &e1);
	
	while(b1.getIsFinished() == 0){
		cout << p1.getName() << "'s Hp: " << p1.getCurrentHp() << "/" << p1.getMaxHp() << endl;
		cout << e1.getName() << "'s Hp: " << e1.getCurrentHp() << "/" << e1.getMaxHp() << endl;
		cout << "--------------------------------------" << endl << endl;
		b1.attackEnemy();
		b1.attackPlayer();
		b1.roundIncrement();
		b1.checkDeaths();
		cout << "--------------------------------------" << endl;
	}
}
