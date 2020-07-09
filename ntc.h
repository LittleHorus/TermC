// Значение температуры, возвращаемое если сумма результатов АЦП больше первого значения таблицы
#define TEMPERATURE_UNDER 1550
// Значение температуры, возвращаемое если сумма результатов АЦП меньше последнего значения таблицы
#define TEMPERATURE_OVER -550
// Значение температуры соответствующее первому значению таблицы
#define TEMPERATURE_TABLE_START 1550
// Шаг таблицы 
#define TEMPERATURE_TABLE_STEP -5

// Метод доступа к элементу таблицы, должна соответствовать temperature_table_entry_type
#define TEMPERATURE_TABLE_READ(i) termo_table[i]



int16_t calc_temperature(uint16_t adcsum);
