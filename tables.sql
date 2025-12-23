DROP TABLE IF EXISTS attendance CASCADE;
DROP TABLE IF EXISTS training_sessions CASCADE;
DROP TABLE IF EXISTS memberships CASCADE;
DROP TABLE IF EXISTS trainers CASCADE;
DROP TABLE IF EXISTS members CASCADE;

CREATE TABLE members (
    member_id   SERIAL PRIMARY KEY,
    full_name   VARCHAR(100) NOT NULL,
    birth_date  DATE NOT NULL,
    phone       VARCHAR(30) NOT NULL,
    email       VARCHAR(120) UNIQUE
);

CREATE TABLE memberships (
    membership_id SERIAL PRIMARY KEY,
    member_id     INT NOT NULL REFERENCES members(member_id) ON DELETE CASCADE,
    plan_type     VARCHAR(30) NOT NULL CHECK (plan_type IN ('месячный','квартальный','годовой','студент')),
    start_date    DATE NOT NULL,
    end_date      DATE NOT NULL,
    price         NUMERIC(10,2) NOT NULL CHECK (price >= 0),
    is_active     BOOLEAN NOT NULL DEFAULT TRUE,
    CHECK (end_date >= start_date)
);

CREATE TABLE trainers (
    trainer_id      SERIAL PRIMARY KEY,
    full_name       VARCHAR(100) NOT NULL,
    specialization  VARCHAR(60) NOT NULL,
    phone           VARCHAR(30),
    salary          NUMERIC(10,2) NOT NULL CHECK (salary >= 0)
);

CREATE TABLE training_sessions (
    session_id        SERIAL PRIMARY KEY,
    trainer_id        INT NOT NULL REFERENCES trainers(trainer_id) ON DELETE RESTRICT,
    session_name      VARCHAR(80) NOT NULL,
    session_time      TIMESTAMP NOT NULL,
    duration_minutes  INT NOT NULL CHECK (duration_minutes BETWEEN 15 AND 240),
    max_participants  INT NOT NULL CHECK (max_participants BETWEEN 1 AND 50)
);

CREATE TABLE attendance (
    attendance_id SERIAL PRIMARY KEY,
    session_id    INT NOT NULL REFERENCES training_sessions(session_id) ON DELETE CASCADE,
    member_id     INT NOT NULL REFERENCES members(member_id) ON DELETE CASCADE,
    status        VARCHAR(20) NOT NULL CHECK (status IN ('Присутствовал','Отсутствовал','Опоздал')),
    created_at    TIMESTAMP NOT NULL DEFAULT NOW(),
    UNIQUE(session_id, member_id)
);

INSERT INTO members(full_name, birth_date, phone, email) VALUES
('Иван Петров','1999-04-12','+7-913-111-11-11','ivan@mail.ru'),
('Анна Смирнова','2001-10-03','+7-913-222-22-22','anna@mail.ru'),
('Дмитрий Кузнецов','1997-01-25','+7-913-333-33-33','dima@mail.ru'),
('Мария Волкова','2000-06-19','+7-913-444-44-44','maria@mail.ru');

INSERT INTO trainers(full_name, specialization, phone, salary) VALUES
('Олег Сидоров','Силовые тренировки','+7-913-555-55-55',80000),
('Екатерина Орлова','Лечебная физкультура','+7-913-666-66-66',90000),
('Алексей Морозов','Йога','+7-913-777-77-77',70000);

INSERT INTO memberships(member_id, plan_type, start_date, end_date, price, is_active) VALUES
(1,'месячный',   CURRENT_DATE - 10, CURRENT_DATE + 20, 2500, TRUE),
(2,'годовой',    CURRENT_DATE - 100, CURRENT_DATE + 265, 22000, TRUE),
(3,'студент',   CURRENT_DATE - 40, CURRENT_DATE - 5,  1800, TRUE),
(4,'квартальный', CURRENT_DATE - 5,  CURRENT_DATE + 85, 6500, TRUE);

INSERT INTO training_sessions(trainer_id, session_name, session_time, duration_minutes, max_participants) VALUES
(1,'Силовая группа',    NOW() + INTERVAL '1 day', 60, 12),
(2,'ЛФК',        NOW() + INTERVAL '2 day', 45, 15),
(3,'Йога',NOW() + INTERVAL '3 day', 60, 10);

INSERT INTO attendance(session_id, member_id, status) VALUES
(1,1,'Присутствовал'),
(1,2,'Присутствовал'),
(1,4,'Отсутствовал'),
(2,1,'Опоздал'),
(2,2,'Присутствовал'),
(3,2,'Присутствовал'),
(3,4,'Присутствовал');


