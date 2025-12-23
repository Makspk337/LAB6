#!/bin/bash

echo "---Настройка базы данных тренажерного зала---"

echo "Установка пароля для postgres..."
sudo -u postgres psql -c "ALTER USER postgres PASSWORD 'postgres';"

echo "Создание базы данных gym_db..."
sudo -u postgres psql -c "DROP DATABASE IF EXISTS gym_db;"
sudo -u postgres psql -c "CREATE DATABASE gym_db;"

echo "Инициализация схемы..."
sudo -u postgres psql -d gym_db -f schema.sql

echo ""
echo "---Готово!---"
echo "Пароль postgres: postgres"
echo "База данных: gym_db"

