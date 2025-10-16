TRUNCATE TABLE 
    public.issuance,
    public.booking,
    public.book_instance,
    public.book,
    public.reader,
    public.publishing_house,
    public.author,
    public.logs
RESTART IDENTITY CASCADE;
-----------------------------------------------------------
INSERT INTO author (first_name, last_name) VALUES 
('Александр','Пушкин'),
('Михаил','Булгаков'),
('Лев','Толстой'),
('Федор','Достоевский'),
('Антон','Чехов'),
('Николай','Гоголь'),
('Иван','Тургенев'),
('Владимир','Набоков'),
('Сергей','Есенин'),
('Анна','Ахматова')
RETURNING id

UPDATE author SET last_name='Ахматова-Горенко' WHERE first_name='Анна'

DELETE FROM author WHERE id IN (2, 5)

-----------------------------------------------------------

INSERT INTO publishing_house (name, city) VALUES 
('Солнышко','Москва'),
('АСТ','Москва'),
('Эксмо','Санкт-Петербург'),
('Просвещение','Москва'),
('Росмэн','Казань'),
('Дрофа','Москва'),
('Фламинго','Санкт-Петербург'),
('Олма-Пресс','Новосибирск'),
('Азбука','Москва'),
('Амфора','Екатеринбург')
RETURNING id;

UPDATE publishing_house SET name='Просвещение+' WHERE id=4;

DELETE FROM publishing_house WHERE id IN (2, 4, 6);

-----------------------------------------------------------

INSERT INTO book (name_book, author, publisher_code, edition, year_publication, circulation) VALUES
('Колобок', 1, 1, 1, 2012, 5000),
('Маша и медведь', 1, 3, 1, 2002, 5200),
('Мастер и Маргарита', 2, 2, 1, 1966, 10000),
('Война и мир', 3, 3, 1, 1869, 8000),
('Преступление и наказание', 4, 1, 2, 1866, 7000),
('Евгений Онегин', 1, 2, 3, 1833, 12000),
('Мертвые души', 6, 3, 1, 1842, 6000),
('Отцы и дети', 7, 1, 2, 1862, 5500),
('Лолита', 8, 2, 1, 1955, 3000),
('Анна Каренина', 3, 3, 2, 1877, 9000)
RETURNING id;

UPDATE book SET name_book='Маша и три медведя' WHERE id=2;

DELETE FROM book WHERE id IN (9,10);

-----------------------------------------------------------

INSERT INTO reader (first_name, last_name, date_of_birth, gender, date_of_regestration) VALUES
('Даниил', 'Молчанов', '2006-10-25', 'male', CURRENT_DATE),
('Иван', 'Иванов', '1950-10-24', 'male', CURRENT_DATE),
('Алексей', 'Перов', '1980-09-21', 'male', CURRENT_DATE),
('Мария', 'Иванова', '2000-05-15', 'female', '2024-01-10'),
('Ольга', 'Петрова', '1995-12-01', 'female', CURRENT_DATE - INTERVAL '5 days'),
('Сергей', 'Сидоров', '1975-03-18', 'male', CURRENT_DATE - INTERVAL '10 days'),
('Екатерина', 'Кузнецова', '1988-07-30', 'female', CURRENT_DATE - INTERVAL '3 days'),
('Дмитрий', 'Васильев', '1992-11-11', 'male', '2024-01-15'),
('Анастасия', 'Попова', '1999-04-22', 'female', CURRENT_DATE),
('Михаил', 'Соколов', '1965-08-14', 'male', CURRENT_DATE - INTERVAL '7 days')
RETURNING id;

UPDATE reader SET date_of_regestration='2024-01-20' WHERE id=10;

DELETE FROM reader WHERE id IN (1,2,3);

-----------------------------------------------------------

INSERT INTO book_instance (book_information, state, status, location) VALUES
(1, 'good', 'in stock', 'Стеллаж 1'),
(2, 'excellent', 'reserved', 'Стеллаж 2'),
(3, 'good', 'issued', 'Стеллаж 3'),
(4, 'good', 'in stock', 'Стеллаж 4'),
(5, 'satisfactory', 'in stock', 'Стеллаж 5'),
(6, 'excellent', 'in stock', 'Стеллаж 1'),
(7, 'good', 'reserved', 'Стеллаж 2'),
(8, 'satisfactory', 'issued', 'Стеллаж 3'),
(9, 'good', 'in stock', 'Стеллаж 4'),
(10, 'excellent', 'in stock', 'Стеллаж 5')
RETURNING id;

UPDATE book_instance SET status='issued' WHERE id=9;

DELETE FROM book_instance WHERE id IN (4);

-----------------------------------------------------------