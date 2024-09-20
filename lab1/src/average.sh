#!/bin/bash

if [ "$#" -eq 0 ]; then
  echo "Укажите файл с числами в качестве аргумента."
  exit 1
fi
if [ ! -f "$1" ]; then
  echo "Файл '$1' не найден."
  exit 1
fi


sum=0
count=0


while read -r arg; do
  if [[ $arg =~ ^-?[0-9]+$ ]]; then
    sum=$((sum + arg))
    count=$((count + 1))
  else
    echo "Найдены недопустимые данные: $arg"
  fi
done < "$1"

if [ "$count" -eq 0 ]; then
  echo "Нет допустимых чисел для вычисления среднего."
  exit 1
fi

avg_a=$(bc <<< "scale=3; $sum / $count")

echo "Количество: $count"
echo "Среднее арифметическое: $avg_a"