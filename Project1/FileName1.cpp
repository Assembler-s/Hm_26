#include <iostream>
#include <windows.h>

using namespace std;

class Gradebook
{
private:
	int* grades;   // динамічний масив оцінок за заняття (оцінки від 1 до 12; 0 означає «оцінки немає»)
	int count;     // кількість занять

public:
	explicit Gradebook(int count_)
	{
		count = count_;

		if (count < 1)
			count = 1;

		grades = new int[count] {};
	}

	~Gradebook()
	{
		delete[] grades;
	}

	Gradebook(double count) = delete;
	Gradebook(const Gradebook&) = delete;
	Gradebook& operator = (const Gradebook&) = delete;

	friend ostream& operator << (ostream& out, const Gradebook& book);
	friend istream& operator >> (istream& in, Gradebook& book);

	int& operator [] (int lesson)
	{
		return grades[lesson];
	}

	int operator[](int lesson) const
	{
		return grades[lesson];
	}

	int getCount() const
	{
		return count;
	}

	bool setGrade(int lesson, int grade)
	{
		if (lesson < 0 || lesson >= count || grade < 1 || grade > 12)
			return false;

		grades[lesson] = grade;
		return true;
	}

	double average() const
	{
		double result = 0, step = 0;
		for (int i = 0; i < count; i++)
		{
			if (grades[i] != 0)
			{
				result += grades[i];
				step++;
			}		
		}
		if (step != 0)
			return result /= step;
		
		return 0;
	}

	explicit operator bool() const
	{
		for (int i = 0; i < count; i++)
		{
			if (grades[i] != 0)
				return true;
		};
		return false;
	}
};

//istream& operator >> (istream& in, Gradebook& book)
//{
//	in >> book.count;
//	return in;
//}

ostream& operator << (ostream& out, const Gradebook& book)
{
	out << "Оценки: ";
	for (int i = 0; i < book.count; i++)
	{
		if (book.grades[i] == 0)
			out << "- ";
		else
			out << book.grades[i] << " ";
	}

	out << "| средний балл: " << book.average();
	return out;
}

istream& operator>>(istream& in, Gradebook& book)
{
	for (int i = 0; i < book.count; i++)
	{
		int grade;
		in >> grade;

		if (grade < 0 || grade > 12)
			book.grades[i] = 0;
		else
			book.grades[i] = grade;
	}
	return in;
}

int bestGrade(const Gradebook& book)
{
	int size = book.getCount();
	int largest = book[0];

	for (int i = 1; i < size; i++)
	{
		if (book[i] > largest)
			largest = book[i];
	}

	return largest;
}

bool operator<(const Gradebook& first, const Gradebook& second)
{
	return first.average() < second.average();
}

int main()
{
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);

	// Уровеь 1
	Gradebook book{ 5 };
	cout << book << endl;

	if (!book)
		cout << "Оценок ещё нет!\n";

	book[0] = 10;
	book[1] = 11;
	book[2] = 12;
	cout << book << endl;
	cout << "Высший балл: " << bestGrade(book) << endl;

	//Gradebook copy = book;   // Не компилируется, потому что конструктор копирования удалён(= delete).
	//Gradebook wrong(5.5);    // Не компилируется, потому что конструктор с параметром double удалён (= delete).
	///////////////////////////////

	//Уровень 2
	if (!book.setGrade(0, 15))
		cout << "Помилка: оцінка має бути від 1 до 12!\n";

	Gradebook book2{ 5 };
	cout << "Введіть оцінки другого студента: ";
	cin >> book2;
	cout << book2 << endl;
	//////////////////////////////

	// Уровень 3
	
	//////////////////////////////
	if (book < book2)
		cout << "У второго студента средний балл выше.\n";
	else
		cout << "У первого студента средний балл не ниже.\n";


	return 0;
}

/*1) Потому что в cout << book слева стоит cout, а справа — book. Если бы это был метод класса, он вызывался бы у book,
и порядок операндов не совпадал бы. friend позволяет функции обращаться к private-полям класса.*/

/*2) Первая версия возвращает int& и позволяет изменять оценку, вторая возвращает int и только читает её. В bestGrade вызывается 
вторая версия, потому что параметр book объявлен как const Gradebook&.*/

/*3) Потому что [] возвращает прямой доступ к элементу массива, а setGrade() выполняет проверку перед присваиванием. 
Через [] можно записать любое значение, а setGrade() проверяет, что оценка находится в пределах от 1 до 12.*/

/*4) explicit запрещает неявное преобразование Gradebook в bool. Без него скомпилировалась бы, например, такая строка:
bool hasGrades = book; С explicit нужно явно преобразовать объект или использовать его в условии, например if (book).*/

/*5) Создалась бы поверхностная копия: copy и book указывали бы на один динамический массив grades. При уничтожении обоих 
объектов возникло бы двойное освобождение памяти.*/

/*6) Глобальной функцией, потому что она сравнивает средние баллы через публичный метод average(). Доступ к private-полям
Sи friend ей не нужны.*/