-- Keep a log of any SQL queries you execute as you solve the mystery.

-- 1. Get crime scene report for time and place in question:
-- theft took place on July 28, 2021
-- theft took place on Humphrey Street
SELECT description FROM crime_scene_reports WHERE year = '2021' AND month = '7' and street = 'Humphrey Street';

-- important information:
-- theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with
-- three witnesses who were present at the time – each of their interview transcripts
-- mentions the bakery.

-- 2. Search for the three interview/witnesses mentioned in the crime scene report
SELECT name, transcript FROM interviews WHERE transcript LIKE "%bakery%";

-- important information:
-- name of the three witnesses: Ruth, Eugene, Raymond
-- Ruth: Sometime within ten minutes of the theft, thief got into a car in the
-- bakery parking lot and drove away. -> bakery security footage
-- Eugene: Earlier this morning, I was walking by the ATM on Leggett Street and saw the thief there
-- withdrawing some money -> atm transactions
-- Raymond: thief called someone after leaving bakery, talked for less than a minute
-- planning to take the earliest flight out of Fiftyville tomorrow
-- asked the person on the other end of the phone to purchase the flight ticket
-- -> phone calls, airports, flights

-- 3. check bakery security footage between 10:15 and 10:25
SELECT license_plate FROM bakery_security_logs WHERE year = '2021' AND month = '7' AND day = '28' AND hour = '10' AND minute BETWEEN '15' AND '25';
-- license plates:
-- 5P2BI95
-- 94KL13X
-- 6P58WS2
-- 4328GD8
-- G412CB7
-- L93JTIZ
-- 322W7JE
-- 0NTHK55

-- 4. get license plate's owners
SELECT name FROM people WHERE license_plate IN (SELECT license_plate FROM bakery_security_logs WHERE year = '2021' AND month = '7' AND day = '28' AND hour = '10' AND minute BETWEEN '15' AND '25');
-- Vanessa
-- Barry
-- Iman
-- Sofia
-- Luca
-- Diana
-- Kelsey
-- Bruce

-- 5. check ATM transactions from Leggett Street ATM
SELECT account_number FROM atm_transactions WHERE year = '2021' AND month = '7' AND day = '28' AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw';
-- 28500762
-- 28296815
-- 76054385
-- 49610011
-- 16153065
-- 25506511
-- 81061156
-- 26013199

-- 6. get account owner's names of bank accounts above
SELECT name FROM people WHERE id IN (SELECT person_id FROM bank_accounts WHERE account_number IN (SELECT account_number FROM atm_transactions WHERE year = '2021' AND month = '7' AND day = '28' AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw'));
-- Kenny
-- Iman x
-- Benista
-- Taylor
-- Brooke
-- Luca
-- Diana x
-- Bruce x
-- -> Iman, Diana, and Bruce both drove away from parking lot and withdraw money

-- 7. check phone calls
SELECT caller, receiver FROM phone_calls WHERE year = '2021' AND month = '7' AND day = '28' AND duration < '60';
--(130) 555-0289|(996) 555-8899
--(499) 555-9472|(892) 555-8872
--(367) 555-5533|(375) 555-8161
--(499) 555-9472|(717) 555-1342
--(286) 555-6063|(676) 555-6554
--(770) 555-1861|(725) 555-3243
--(031) 555-6622|(910) 555-3251
--(826) 555-1652|(066) 555-9701
--(338) 555-6650|(704) 555-2131

-- 8. get names of callers above
SELECT name FROM people WHERE phone_number IN (SELECT caller FROM phone_calls WHERE year = '2021' AND month = '7' AND day = '28' AND duration < '60');
--Kenny
--Sofia
--Benista
--Taylor
--Diana x
--Kelsey
--Bruce x
--Carina
-- -> Diana and Bruce drove, withdraw, and called

--9. get call receivers of Diana and Bruce
SELECT name FROM people WHERE phone_number IN (SELECT receiver FROM phone_calls WHERE year = '2021' AND month = '7' AND day = '28' AND duration < '60' AND caller IN (SELECT phone_number FROM people WHERE name IN ('Diana', 'Bruce')));
-- Philip
-- Robin

--10. check flights (earliest flight from Fiftyville on 7/29)
SELECT id, hour, minute FROM flights WHERE origin_airport_id IN (SELECT id FROM airports WHERE city = 'Fiftyville') AND year = '2021' AND month = '7' AND day = '29' ORDER BY hour;
--36|8|20
--43|9|30
--23|12|15
--53|15|20
--18|16|0
-- -> earliest flight is flight 36 at 8:20

--11. get passengers of flight 36
SELECT name FROM people WHERE passport_number IN (SELECT passport_number FROM passengers WHERE flight_id = '36');
--Kenny
--Sofia
--Taylor
--Luca
--Kelsey
--Edward
--Bruce x
--Doris
-- -> Bruce seems to be the culprit, Bruce called Robin, so Robin is the accomplice

--12. Flight Destination is NY City:
SELECT city FROM airports WHERE id IN (SELECT destination_airport_id FROM flights WHERE id = '36');