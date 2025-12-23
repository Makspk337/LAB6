SELECT m.full_name, m.phone, ms.plan_type, ms.start_date, ms.end_date, ms.price
FROM members m
INNER JOIN memberships ms ON m.member_id = ms.member_id
WHERE ms.is_active = TRUE
ORDER BY m.full_name;

SELECT plan_type, COUNT(*) AS count, SUM(price) AS revenue
FROM memberships
WHERE is_active = TRUE
GROUP BY plan_type
ORDER BY count DESC;

SELECT t.full_name, t.specialization, COUNT(ts.session_id) AS sessions
FROM trainers t
LEFT JOIN training_sessions ts ON t.trainer_id = ts.trainer_id
GROUP BY t.trainer_id, t.full_name, t.specialization
ORDER BY sessions DESC;

SELECT ts.session_name, t.full_name AS trainer_name, ts.session_time, ts.duration_minutes, ts.max_participants
FROM training_sessions ts
INNER JOIN trainers t ON ts.trainer_id = t.trainer_id
ORDER BY ts.session_time;

SELECT
    ts.session_name AS training_name,
    t.full_name AS trainer_name,
    COUNT(a.attendance_id) AS total_attendances,
    COUNT(CASE WHEN a.status IN ('Присутствовал','Опоздал') THEN 1 END) AS attended_count,
    COUNT(CASE WHEN a.status = 'Отсутствовал' THEN 1 END) AS absent_count
FROM training_sessions ts
INNER JOIN trainers t ON ts.trainer_id = t.trainer_id
LEFT JOIN attendance a ON ts.session_id = a.session_id
GROUP BY ts.session_id, ts.session_name, t.full_name
ORDER BY attended_count DESC;


SELECT DISTINCT m.full_name, m.phone, m.email
FROM members m
WHERE m.member_id IN (
    SELECT DISTINCT member_id
    FROM attendance
    WHERE status='Присутствовал'
)
ORDER BY m.full_name;

SELECT specialization,
       COUNT(*) AS trainers_count,
       AVG(salary) AS avg_salary,
       MIN(salary) AS min_salary,
       MAX(salary) AS max_salary
FROM trainers
GROUP BY specialization
HAVING COUNT(*) >= 1
ORDER BY avg_salary DESC;

SELECT m.full_name, m.phone, ms.plan_type, ms.end_date
FROM members m
INNER JOIN memberships ms ON m.member_id = ms.member_id
WHERE ms.end_date < CURRENT_DATE AND ms.is_active = TRUE
ORDER BY ms.end_date;

SELECT ts.session_name, t.full_name AS trainer_name,
       ts.max_participants,
       COUNT(CASE WHEN a.status='Присутствовал' THEN 1 END) AS attended_count,
       ROUND((COUNT(CASE WHEN a.status='Присутствовал' THEN 1 END)::numeric / NULLIF(ts.max_participants,0)) * 100, 1) AS fill_percent
FROM training_sessions ts
INNER JOIN trainers t ON ts.trainer_id = t.trainer_id
LEFT JOIN attendance a ON ts.session_id = a.session_id
GROUP BY ts.session_id, ts.session_name, t.full_name, ts.max_participants
ORDER BY fill_percent DESC NULLS LAST;

SELECT m.full_name,
       COUNT(a.attendance_id) AS total_visits,
       MAX(ms.plan_type) AS membership_type
FROM members m
INNER JOIN attendance a ON m.member_id = a.member_id
LEFT JOIN memberships ms ON m.member_id = ms.member_id AND ms.is_active = TRUE
WHERE a.status='Присутствовал'
GROUP BY m.member_id, m.full_name
ORDER BY total_visits DESC
LIMIT 3;


