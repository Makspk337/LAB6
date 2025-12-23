#include <iostream>
#include <pqxx/pqxx>
#include <string>
#include <iomanip>

using namespace std;
using namespace pqxx;

class GymDB {
private:
    string connection_string;

public:
    GymDB(const string& conn_str) : connection_string(conn_str) {}

    connection connect() {
        try {
            connection C(connection_string);
            if (C.is_open()) {
                cout << "Подключение к БД успешно: " << C.dbname() << endl;
            }
            return C;
        } catch (const exception &e) {
            cerr << "Ошибка подключения: " << e.what() << endl;
            throw;
        }
    }

    void getActiveMembers() {
        cout << "\n----Активные абонементы----" << endl;

        try {
            connection C = connect();
            work W(C);

            result R = W.exec(
                "SELECT m.full_name, m.phone, ms.plan_type, ms.start_date, ms.end_date, ms.price "
                "FROM members m "
                "INNER JOIN memberships ms ON m.member_id = ms.member_id "
                "WHERE ms.is_active = TRUE "
                "ORDER BY m.full_name"
            );

            cout << left << setw(25) << "Имя"
                 << " " << left << setw(15) << "Телефон"
                 << " " << left << setw(15) << "Тип"
                 << " " << left << setw(12) << "Начало"
                 << " " << left << setw(12) << "Конец"
                 << " " << right << setw(10) << "Цена" << endl;
            cout << string(95, '-') << endl;

            for (auto row : R) {
                cout << left << setw(25) << row[0].c_str()
                     << " " << left << setw(15) << row[1].c_str()
                     << " " << left << setw(15) << row[2].c_str()
                     << " " << left << setw(12) << row[3].c_str()
                     << " " << left << setw(12) << row[4].c_str()
                     << " " << right << setw(10) << row[5].c_str() << endl;
            }

            W.commit();
            C.close();
        } catch (const exception &e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    void getMembershipStats() {
        cout << "\n----Статистика по типам абонементов----" << endl;

        try {
            connection C = connect();
            work W(C);

            result R = W.exec(
                "SELECT plan_type, COUNT(*) AS count, SUM(price) AS revenue "
                "FROM memberships "
                "WHERE is_active = TRUE "
                "GROUP BY plan_type "
                "ORDER BY count DESC"
            );

            cout << left << setw(20) << "Тип"
                 << " " << right << setw(20) << "Количество"
                 << " " << right << setw(15) << "Доход" << endl;
            cout << string(52, '-') << endl;

            for (auto row : R) {
                cout << left << setw(20) << row[0].c_str()
                     << " " << right << setw(20) << row[1].c_str()
                     << " " << right << setw(15) << row[2].c_str() << endl;
            }

            W.commit();
            C.close();
        } catch (const exception &e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    void getTrainerWorkload() {
        cout << "\n----Нагрузка тренеров----" << endl;

        try {
            connection C = connect();
            work W(C);

            result R = W.exec(
                "SELECT t.full_name, t.specialization, COUNT(ts.session_id) AS sessions "
                "FROM trainers t "
                "LEFT JOIN training_sessions ts ON t.trainer_id = ts.trainer_id "
                "GROUP BY t.trainer_id, t.full_name, t.specialization "
                "ORDER BY sessions DESC"
            );

            cout << left << setw(25) << "Тренер"
                 << " " << left << setw(25) << "Специализация"
                 << " " << right << setw(12) << "Сессий" << endl;
            cout << string(68, '-') << endl;

            for (auto row : R) {
                cout << left << setw(25) << row[0].c_str()
                     << " " << left << setw(25) << row[1].c_str()
                     << " " << right << setw(12) << row[2].c_str() << endl;
            }

            W.commit();
            C.close();
        } catch (const exception &e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    void getTrainingSchedule() {
        cout << "\n----Расписание тренировок----" << endl;

        try {
            connection C = connect();
            work W(C);

            result R = W.exec(
                "SELECT ts.session_name, t.full_name AS trainer_name, "
                "ts.session_time, ts.duration_minutes, ts.max_participants "
                "FROM training_sessions ts "
                "INNER JOIN trainers t ON ts.trainer_id = t.trainer_id "
                "ORDER BY ts.session_time"
            );

            cout << left << setw(22) << "Тренировка"
                 << " " << left << setw(22) << "Тренер"
                 << " " << left << setw(20) << "Время"
                 << " " << right << setw(10) << "Мин"
                 << " " << right << setw(10) << "Макс" << endl;
            cout << string(95, '-') << endl;

            for (auto row : R) {
                cout << left << setw(28) << row[0].c_str()
                     << " " << left << setw(22) << row[1].c_str()
                     << " " << left << setw(20) << row[2].c_str()
                     << " " << right << setw(10) << row[3].c_str()
                     << " " << right << setw(10) << row[4].c_str() << endl;
            }

            W.commit();
            C.close();
        } catch (const exception &e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    void getAttendanceStats() {
        cout << "\n----Посещаемость по тренировкам----" << endl;

        try {
            connection C = connect();
            work W(C);

            result R = W.exec(
                "SELECT ts.session_name AS training_name, t.full_name AS trainer_name, "
                "COUNT(a.attendance_id) AS total_attendances, "
                "COUNT(CASE WHEN a.status IN ('Присутствовал','Опоздал') THEN 1 END) AS attended_count, "
                "COUNT(CASE WHEN a.status = 'Отсутствовал' THEN 1 END) AS absent_count "
                "FROM training_sessions ts "
                "INNER JOIN trainers t ON ts.trainer_id = t.trainer_id "
                "LEFT JOIN attendance a ON ts.session_id = a.session_id "
                "GROUP BY ts.session_id, ts.session_name, t.full_name "
                "ORDER BY attended_count DESC"
            );

            cout << left << setw(28) << "Тренировка"
                 << " " << left << setw(22) << "Тренер"
                 << " " << right << setw(8) << "Всего"
                 << " " << right << setw(8) << "Пришли"
                 << " " << right << setw(10) << "Не пришли" << endl;
            cout << string(82, '-') << endl;

            for (auto row : R) {
                cout << left << setw(28) << row[0].c_str()
                     << " " << left << setw(22) << row[1].c_str()
                     << " " << right << setw(8) << row[2].c_str()
                     << " " << right << setw(8) << row[3].c_str()
                     << " " << right << setw(10) << row[4].c_str() << endl;
            }

            W.commit();
            C.close();
        } catch (const exception &e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    void getActiveAttendees() {
        cout << "\n----Участники, посещавшие тренировки----" << endl;

        try {
            connection C = connect();
            work W(C);

            result R = W.exec(
                "SELECT DISTINCT m.full_name, m.phone, m.email "
                "FROM members m "
                "WHERE m.member_id IN ( "
                "  SELECT DISTINCT member_id "
                "  FROM attendance "
                "  WHERE status IN ('Присутствовал','Опоздал') "
                ") "
                "ORDER BY m.full_name"
            );

            cout << left << setw(25) << "Имя"
                 << " " << left << setw(15) << "Телефон"
                 << " " << left << setw(35) << "Email" << endl;
            cout << string(80, '-') << endl;

            for (auto row : R) {
                cout << left << setw(25) << row[0].c_str()
                     << " " << left << setw(15) << row[1].c_str()
                     << " " << left << setw(35) << row[2].c_str() << endl;
            }

            W.commit();
            C.close();
        } catch (const exception &e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    void getTrainerSalaryStats() {
        cout << "\n----Статистика зарплат тренеров----" << endl;

        try {
            connection C = connect();
            work W(C);

            result R = W.exec(
                "SELECT specialization, COUNT(*) AS trainers_count, "
                "AVG(salary) AS avg_salary, MIN(salary) AS min_salary, MAX(salary) AS max_salary "
                "FROM trainers "
                "GROUP BY specialization "
                "HAVING COUNT(*) >= 1 "
                "ORDER BY avg_salary DESC"
            );

            cout << left << setw(22) << "Специализация"
                 << " " << right << setw(8) << "Кол-во"
                 << " " << right << setw(12) << "Средняя"
                 << " " << right << setw(10) << "Мин"
                 << " " << right << setw(10) << "Макс" << endl;
            cout << string(70, '-') << endl;

            for (auto row : R) {
                cout << left << setw(22) << row[0].c_str()
                     << " " << right << setw(8) << row[1].c_str()
                     << " " << right << setw(12) << row[2].c_str()
                     << " " << right << setw(10) << row[3].c_str()
                     << " " << right << setw(10) << row[4].c_str() << endl;
            }

            W.commit();
            C.close();
        } catch (const exception &e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    void getExpiredMemberships() {
        cout << "\n----Участники с истекшими абонементами----" << endl;

        try {
            connection C = connect();
            work W(C);

            result R = W.exec(
                "SELECT m.full_name, m.phone, ms.plan_type, ms.end_date "
                "FROM members m "
                "INNER JOIN memberships ms ON m.member_id = ms.member_id "
                "WHERE ms.end_date < CURRENT_DATE AND ms.is_active = TRUE "
                "ORDER BY ms.end_date"
            );

            cout << left << setw(25) << "Имя"
                 << " " << left << setw(15) << "Телефон"
                 << " " << left << setw(15) << "Тип"
                 << " " << left << setw(12) << "Истек" << endl;
            cout << string(72, '-') << endl;

            if (R.size() == 0) cout << "Нет истекших абонементов" << endl;

            for (auto row : R) {
                cout << left << setw(25) << row[0].c_str()
                     << " " << left << setw(15) << row[1].c_str()
                     << " " << left << setw(15) << row[2].c_str()
                     << " " << left << setw(12) << row[3].c_str() << endl;
            }

            W.commit();
            C.close();
        } catch (const exception &e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    void getSessionFillRates() {
        cout << "\n----Заполняемость тренировок----" << endl;

        try {
            connection C = connect();
            work W(C);

            result R = W.exec(
                "SELECT ts.session_name, t.full_name AS trainer_name, "
                "ts.max_participants, "
                "COUNT(CASE WHEN a.status='attended' THEN 1 END) AS attended_count, "
                "ROUND( (COUNT(CASE WHEN a.status='attended' THEN 1 END)::numeric "
                "       / NULLIF(ts.max_participants,0)) * 100, 1) AS fill_percent "
                "FROM training_sessions ts "
                "INNER JOIN trainers t ON ts.trainer_id = t.trainer_id "
                "LEFT JOIN attendance a ON ts.session_id = a.session_id "
                "GROUP BY ts.session_id, ts.session_name, t.full_name, ts.max_participants "
                "ORDER BY fill_percent DESC NULLS LAST"
            );

            cout << left << setw(28) << "Тренировка"
                 << " " << left << setw(22) << "Тренер"
                 << " " << right << setw(8) << "Макс"
                 << " " << right << setw(10) << "Пришло"
                 << " " << right << setw(10) << "% заполненности" << endl;
            cout << string(86, '-') << endl;

            for (auto row : R) {
                cout << left << setw(28) << row[0].c_str()
                     << " " << left << setw(22) << row[1].c_str()
                     << " " << right << setw(8) << row[2].c_str()
                     << " " << right << setw(10) << row[3].c_str()
                     << " " << right << setw(10) << (row[4].is_null() ? "-" : row[4].c_str())
                     << endl;
            }

            W.commit();
            C.close();
        } catch (const exception &e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    void getTopMembers() {
        cout << "\n----Топ-3 самых активных участника----" << endl;

        try {
            connection C = connect();
            work W(C);

            result R = W.exec(
                "SELECT m.full_name, COUNT(a.attendance_id) AS total_visits, "
                "MAX(ms.plan_type) AS membership_type "
                "FROM members m "
                "INNER JOIN attendance a ON m.member_id = a.member_id "
                "LEFT JOIN memberships ms ON m.member_id = ms.member_id AND ms.is_active = TRUE "
                "WHERE a.status IN ('Присутствовал','Опоздал') AND ms.is_active = TRUE "
                "GROUP BY m.member_id, m.full_name "
                "ORDER BY total_visits DESC "
                "LIMIT 3"
            );

            cout << left << setw(25) << "Имя"
                 << " " << right << setw(12) << "Посещений"
                 << " " << left << setw(18) << "Абонемент" << endl;
            cout << string(60, '-') << endl;

            for (auto row : R) {
                cout << left << setw(25) << row[0].c_str()
                     << " " << right << setw(12) << row[1].c_str()
                     << " " << left << setw(18) << (row[2].is_null() ? "-" : row[2].c_str()) << endl;
            }

            W.commit();
            C.close();
        } catch (const exception &e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    void addMember(const string& name, const string& birth_date,
                   const string& phone, const string& email) {
        cout << "\n----Добавление нового участника----" << endl;

        try {
            connection C = connect();
            work W(C);

            C.prepare("insert_member",
                "INSERT INTO members (full_name, birth_date, phone, email) "
                "VALUES ($1, $2, $3, $4) RETURNING member_id");

            result R = W.exec_prepared("insert_member", name, birth_date, phone, email);

            cout << "Участник добавлен. ID: " << R[0][0].c_str() << endl;

            W.commit();
            C.close();
        } catch (const exception &e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    void unsafeQuery(const string& name) {
        cout << "\n----Небезопасный запрос----" << endl;
        cout << "Ищем участника: " << name << endl;

        try {
            connection C = connect();
            nontransaction N(C);

            string query = "SELECT member_id, full_name FROM members WHERE full_name = '" + name + "'";
            cout << "Запрос: " << query << endl;

            result R = N.exec(query);

            cout << "Найдено записей: " << R.size() << endl;
            for (auto row : R) {
                cout << "ID: " << row[0].c_str() << ", Имя: " << row[1].c_str() << endl;
            }

            C.close();
        } catch (const exception &e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }

    void safeQuery(const string& name) {
        cout << "\n----Безопасный запрос----" << endl;

        try {
            connection C = connect();
            work W(C);

            C.prepare("safe_select",
                "SELECT member_id, full_name FROM members WHERE full_name = $1");

            result R = W.exec_prepared("safe_select", name);

            cout << "Найдено записей: " << R.size() << endl;

            for (auto row : R) {
                cout << "ID: " << row[0].c_str()
                     << ", Имя: " << row[1].c_str() << endl;
            }

            W.commit();
            C.close();
        } catch (const exception &e) {
            cerr << "Ошибка: " << e.what() << endl;
        }
    }
};

void printMenu() {
    cout << "\n--------Тренажерный зал--------" << endl;
    cout << "1.  Показать активные абонементы" << endl;
    cout << "2.  Статистика по абонементам" << endl;
    cout << "3.  Нагрузка тренеров" << endl;
    cout << "4.  Расписание тренировок" << endl;
    cout << "5.  Посещаемость по тренировкам" << endl;
    cout << "6.  Участники, посещавшие тренировки" << endl;
    cout << "7.  Статистика зарплат тренеров" << endl;
    cout << "8.  Участники с истекшими абонементами" << endl;
    cout << "9.  Заполняемость тренировок" << endl;
    cout << "10. Топ-3 самых активных участника" << endl;
    cout << "11. Добавить участника" << endl;
    cout << "12. Небезопасный запрос" << endl;
    cout << "13. Безопасный запрос" << endl;
    cout << "0.  Выход" << endl;
    cout << "Выбор: ";
}

int main() {
    string conn_str = "dbname=gym_db host=localhost port=5432";

    GymDB db(conn_str);

    int choice;
    bool running = true;

    while (running) {
        printMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:  db.getActiveMembers(); break;
            case 2:  db.getMembershipStats(); break;
            case 3:  db.getTrainerWorkload(); break;
            case 4:  db.getTrainingSchedule(); break;
            case 5:  db.getAttendanceStats(); break;
            case 6:  db.getActiveAttendees(); break;
            case 7:  db.getTrainerSalaryStats(); break;
            case 8:  db.getExpiredMemberships(); break;
            case 9:  db.getSessionFillRates(); break;
            case 10: db.getTopMembers(); break;

            case 11: {
                string name, birth, phone, email;
                cout << "Имя: ";
                getline(cin, name);
                cout << "Дата рождения (YYYY-MM-DD): ";
                getline(cin, birth);
                cout << "Телефон: ";
                getline(cin, phone);
                cout << "Email: ";
                getline(cin, email);
                db.addMember(name, birth, phone, email);
                break;
            }

            case 12: {
                cout << "Введите имя (попробуйте: ' OR '1'='1): ";
                string name;
                getline(cin, name);
                db.unsafeQuery(name);
                break;
            }

            case 13: {
                cout << "Введите имя: ";
                string name;
                getline(cin, name);
                db.safeQuery(name);
                break;
            }

            case 0:
                running = false;
                cout << "Выход..." << endl;
                break;

            default:
                cout << "Неверный выбор" << endl;
        }
    }

    return 0;
}

