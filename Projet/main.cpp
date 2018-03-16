#include <QApplication>
#include "MainWindow.h"
#include "Login.h"
#include "c_init_bd.h"

DBConnect * db;

int main(int argc, char * argv[])
{
	QApplication a(argc, argv);
	
    std::cout << "Création de la DBB: " << (C_INIT_BD::Creation_BD() ? "true" : "false") << std::endl;
	db = new DBConnect();
	
	MainWindow w;
	Login login(&w);
	int result = 0;
	if(login.exec() == QDialog::Accepted) //If user is successfully logged in
	{
		w.show();
		result = a.exec();
	}
	delete db;
	return result;
}
