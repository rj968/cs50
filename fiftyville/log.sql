-- Keep a log of any SQL queries you execute as you solve the mystery.
-- the theft of duck took place on July 28, 2024 on Humphrey Street.
1. SELECT id, description FROM crime_scene_reports WHERE
   ...> year = 2024 AND month = 7 AND day = 28
   ...> AND street = 'Humphrey Street';

   -- case id 295 at 10:15 am

2. SELECT id,name,transcript FROM interviews
   ...> WHERE year = 2024
   ...> AND month = 7
   ...> AND day = 28
   ...> AND transcript LIKE '%bakery%';

   /*| 161 | Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
| 162 | Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
| 163 | Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |*/

3. SELECT DISTINCT activity FROM bakery_security_logs;
+----------+
| activity |
+----------+
| entrance |
| exit     |
+----------+

SELECT license_plate FROM bakery_security_logs WHERE
   ...> year = 2024
   ...> AND month = 07
   ...> AND day = 28
   ...> AND activity ='exit'
   ...> AND hour = 10
   ...> AND minute BETWEEN 15 AND 25;

+---------------+
| license_plate |
+---------------+
| 5P2BI95       |
| 94KL13X       |
| 6P58WS2       |
| 4328GD8       |
| G412CB7       |
| L93JTIZ       |
| 322W7JE       |
| 0NTHK55       |
+---------------+

4. SELECT account_number, amount FROM atm_transactions WHERE year = 2024
    AND month = 7
    AND day = 28
    AND atm_location = 'Leggett Street'
    AND transaction_type = 'withdraw';
+----------------+--------+
| account_number | amount |
+----------------+--------+
| 28500762       | 48     |
| 28296815       | 20     |
| 76054385       | 60     |
| 49610011       | 50     |
| 16153065       | 80     |
| 25506511       | 20     |
| 81061156       | 30     |
| 26013199       | 35     |
+----------------+--------+

5. SELECT id,name,phone_number,passport_number FROM people WHERE
    id IN (SELECT person_id FROM bank_accounts WHERE account_number IN(
        SELECT account_number FROM atm_transactions WHERE year = 2024
            AND month = 7 AND day = 28
            AND atm_location = 'Leggett Street'
            AND transaction_type = 'withdraw'
    ))
    AND
     license_plate IN(
        SELECT license_plate FROM bakery_security_logs WHERE
        year = 2024
        AND month = 07
        AND day = 28
        AND activity ='exit'
        AND hour = 10
        AND minute BETWEEN 15 AND 25
    );

+--------+-------+----------------+-----------------+
|   id   | name  |  phone_number  | passport_number |
+--------+-------+----------------+-----------------+
| 396669 | Iman  | (829) 555-5269 | 7049073643      |
| 467400 | Luca  | (389) 555-5198 | 8496433585      |
| 514354 | Diana | (770) 555-1861 | 3592750733      |
| 686048 | Bruce | (367) 555-5533 | 5773159633      |
+--------+-------+----------------+-----------------+

6. SELECT caller,receiver FROM phone_calls WHERE caller IN(
    SELECT phone_number FROM people WHERE
    id IN (SELECT person_id FROM bank_accounts WHERE account_number IN(
        SELECT account_number FROM atm_transactions WHERE year = 2024
            AND month = 7 AND day = 28
            AND atm_location = 'Leggett Street'
            AND transaction_type = 'withdraw'
    ))
    AND
     license_plate IN(
        SELECT license_plate FROM bakery_security_logs WHERE
        year = 2024
        AND month = 07
        AND day = 28
        AND activity ='exit'
        AND hour = 10
        AND minute BETWEEN 15 AND 25
    )
)
AND year = 2024
AND month = 7
AND day = 28
AND duration < 60;
+----------------+----------------+
|     caller     |    receiver    |
+----------------+----------------+
| (367) 555-5533 | (375) 555-8161 |
| (770) 555-1861 | (725) 555-3243 |
+----------------+----------------+
--check both of their bank withdrawals for 28 and 29

7. SELECT id,name,phone_number FROM people WHERE phone_number IN ('(375) 555-8161','(725) 555-3243','(367) 555-5533','(770) 555-1861');
+--------+--------+----------------+
|   id   |  name  |  phone_number  |
+--------+--------+----------------+
| 514354 | Diana  | (770) 555-1861 |
| 686048 | Bruce  | (367) 555-5533 |
| 847116 | Philip | (725) 555-3243 |
| 864400 | Robin  | (375) 555-8161 |
+--------+--------+----------------+

8. SELECT id,hour,minute FROM flights WHERE origin_airport_id = (
    SELECT id FROM airports WHERE city = 'Fiftyv
ille')
    AND day = 29
    AND month = 7
   AND year = 2024;
+----+------+--------+
| id | hour | minute |
+----+------+--------+
| 18 | 16   | 0      |
| 23 | 12   | 15     |
| 36 | 8    | 20     |
| 43 | 9    | 30     |
| 53 | 15   | 20     |
+----+------+--------+

9. SELECT id,name FROM people WHERE passport_number IN(
    SELECT passport_number FROM passengers WHERE flight_id = 36
    AND passport_number IN(
        SELECT passport_number FROM people WHERE phone_number IN (
            '(375) 555-8161','(725) 555-3243','(367) 555-5533','(770) 555-1861')
    )
);

10. SELECT passport_number FROM passengers WHERE flight_id = 36
   ...>     AND passport_number IN(
   ...>         SELECT passport_number FROM people WHERE phone_number IN (
   ...>             '(375) 555-8161','(725) 555-3243','(367) 555-5533','(770) 555-1861')
   ...>     )
   ...> );
+--------+-------+
|   id   | name  |
+--------+-------+
| 686048 | Bruce |
+--------+-------+


Hence Bruce was the criminal and from our investigation we can conclude that Robin was his accomplice
As for the city

11. SELECT city FROM airports WHERE id =
   ...> (
   ...> SELECT destination_airport_id FROM flights WHERE id = 36);
+---------------+
|     city      |
+---------------+
| New York City |
+---------------+
