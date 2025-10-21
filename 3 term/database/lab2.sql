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


SELECT * FROM author;
SELECT * FROM publishing_house;
SELECT * FROM book;
SELECT * FROM reader;
SELECT * FROM book_instance;
SELECT * FROM issuance;


-----------------------------------------------------------

insert into public.issuance (reader_id, book_id, date_time,date_return_expected,date_return_fact)
values(2,2,CURRENT_TIMESTAMP(0),CURRENT_DATE+ interval '14 days',null)
returning reader_id, book_id;

update public.book_instance
set status='issued'
where id=2;



-----------------------------------------------------------
update public.book_instance
set status='in stock'
where id=2;


update public.issuance
set date_return_fact=CURRENT_DATE
where book_id=2 and reader_id=2;

-----------------------------------------------------------


insert into public.issuance (reader_id, book_id, date_time,date_return_expected,date_return_fact)
values
	(1,2,CURRENT_TIMESTAMP(0)- interval '3 days', CURRENT_DATE+ interval '11 days',null),
	(2,3,CURRENT_TIMESTAMP(0)- interval '8 days', CURRENT_DATE+ interval '2 days',null);

UPDATE public.book_instance
set status = 'issued'
WHERE id IN (2,3);


-----------------------------------------------------------

CREATE OR REPLACE VIEW public.issued_books_view AS
SELECT 
    r.last_name AS reader_last_name,
    r.first_name AS reader_first_name,
    a.last_name AS author_last_name,
    a.first_name AS author_first_name,
    b.name_book AS book_title,
    bi.state AS book_condition,
    i.date_time AS issue_date
FROM public.issuance i
JOIN public.reader r ON i.reader_id = r.id
JOIN public.book_instance bi ON i.book_id = bi.id
JOIN public.book b ON bi.book_information = b.id
JOIN public.author a ON b.author = a.id
WHERE i.date_return_fact IS NULL;

SELECT * FROM public.issued_books_view;



-----------------------------------------------------------


CREATE OR REPLACE VIEW public.overdue_books_view AS
SELECT 
    r.last_name AS reader_last_name,
    r.first_name AS reader_first_name,
    a.last_name AS author_last_name,
    a.first_name AS author_first_name,
    b.name_book AS book_title,
    (CURRENT_DATE-i.date_return_expected) as overdue_days
FROM public.issuance i
JOIN public.reader r ON i.reader_id = r.id
JOIN public.book_instance bi ON i.book_id = bi.id
JOIN public.book b ON bi.book_information = b.id
JOIN public.author a ON b.author = a.id
WHERE i.date_return_fact IS null
and i.date_return_expected<CURRENT_DATE;

SELECT * from public.overdue_books_view

-----------------------------------------------------------



CREATE OR REPLACE FUNCTION reserve_book(
    p_reader_id INTEGER,
    p_book_id INTEGER
) RETURNS BOOLEAN AS $$
DECLARE
    v_available_instance_id INTEGER;
    v_booking_count INTEGER;
    v_reservation_end_date DATE;
BEGIN
 
    SELECT COUNT(*) INTO v_booking_count 
    FROM public.booking 
    WHERE number_reader_ticket = p_reader_id 
    AND data_time + INTERVAL '3 days' > CURRENT_TIMESTAMP;
    
    IF v_booking_count >= 3 THEN
        RAISE EXCEPTION 'Превышено максимальное количество активных бронирований';
    END IF;
    
    SELECT bi.id INTO v_available_instance_id
    FROM public.book_instance bi
    WHERE bi.book_information = p_book_id 
    AND bi.status = 'in stock'
    AND bi.state >= 'satisfactory' 
    LIMIT 1;
    
    IF v_available_instance_id IS NULL THEN
        RAISE EXCEPTION 'Нет доступных экземпляров для бронирования';
    END IF;
    
    v_reservation_end_date := CURRENT_DATE + INTERVAL '3 days';
    
    INSERT INTO public.booking (
        number_reader_ticket, 
        book_information, 
        data_time,
        min_state
    ) VALUES (
        p_reader_id, 
        p_book_id, 
        CURRENT_TIMESTAMP,
        'satisfactory'
    );
    
    UPDATE public.book_instance 
    SET status = 'reserved' 
    WHERE id = v_available_instance_id;
    
    RETURN TRUE;
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------

CREATE OR REPLACE FUNCTION cancel_reservation(
    p_reader_id INTEGER,
    p_book_id INTEGER
) RETURNS BOOLEAN AS $$
DECLARE
    v_booking_id INTEGER;
    v_instance_id INTEGER;
BEGIN
    SELECT b.id INTO v_booking_id
    FROM public.booking b
    WHERE b.number_reader_ticket = p_reader_id 
    AND b.book_information = p_book_id
    AND b.data_time + INTERVAL '3 days' > CURRENT_TIMESTAMP
    LIMIT 1;
    
    IF v_booking_id IS NULL THEN
        RAISE EXCEPTION 'Активное бронирование не найдено';
    END IF;
    
    SELECT bi.id INTO v_instance_id
    FROM public.book_instance bi
    WHERE bi.book_information = p_book_id 
    AND bi.status = 'reserved'
    LIMIT 1;
    
    DELETE FROM public.booking WHERE id = v_booking_id;
    
    IF v_instance_id IS NOT NULL THEN
        UPDATE public.book_instance 
        SET status = 'in stock' 
        WHERE id = v_instance_id;
    END IF;
    
    RETURN TRUE;
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------


CREATE OR REPLACE FUNCTION issue_book(
    p_reader_id INTEGER,
    p_instance_id INTEGER
) RETURNS BOOLEAN AS $$
DECLARE
    v_instance_status availability;
    v_is_reserved BOOLEAN;
    v_reservation_reader_id INTEGER;
BEGIN
    SELECT status INTO v_instance_status
    FROM public.book_instance 
    WHERE id = p_instance_id;
    
    IF v_instance_status IS NULL THEN
        RAISE EXCEPTION 'Экземпляр книги не найден';
    END IF;
    
    IF v_instance_status = 'reserved' THEN
        SELECT number_reader_ticket INTO v_reservation_reader_id
        FROM public.booking b
        JOIN public.book_instance bi ON bi.book_information = b.book_information
        WHERE bi.id = p_instance_id
        AND b.data_time + INTERVAL '3 days' > CURRENT_TIMESTAMP
        LIMIT 1;
        
        IF v_reservation_reader_id IS NOT NULL AND v_reservation_reader_id != p_reader_id THEN
            RAISE EXCEPTION 'Экземпляр забронирован другим читателем';
        END IF;
    END IF;
    
    IF v_instance_status IN ('in stock', 'reserved') THEN
        INSERT INTO public.issuance (
            reader_id, 
            book_id, 
            date_time, 
            date_return_expected
        ) VALUES (
            p_reader_id, 
            p_instance_id, 
            CURRENT_TIMESTAMP,
            CURRENT_DATE + INTERVAL '14 days'
        );
        
        UPDATE public.book_instance 
        SET status = 'issued' 
        WHERE id = p_instance_id;
        
        DELETE FROM public.booking 
        WHERE number_reader_ticket = p_reader_id 
        AND book_information = (SELECT book_information FROM book_instance WHERE id = p_instance_id);
        
        RETURN TRUE;
    ELSE
        RAISE EXCEPTION 'Экземпляр недоступен для выдачи';
    END IF;
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------


CREATE OR REPLACE FUNCTION get_book_locations(p_book_id INTEGER)
RETURNS TABLE(
    location VARCHAR,
    book_state state,
    instance_count BIGINT,
    status availability
) AS $$
BEGIN
    RETURN QUERY
    SELECT 
        bi.location,
        bi.state,
        COUNT(*) as instance_count,
        bi.status
    FROM public.book_instance bi
    WHERE bi.book_information = p_book_id
    GROUP BY bi.location, bi.state, bi.status
    ORDER BY 
        CASE bi.state
            WHEN 'excellent' THEN 1
            WHEN 'good' THEN 2
            WHEN 'satisfactory' THEN 3
            WHEN 'dilapidated' THEN 4
            WHEN 'lost' THEN 5
            ELSE 6
        END,
        bi.location;
END;
$$ LANGUAGE plpgsql;

-----------------------------------------------------------

CREATE OR REPLACE VIEW public.available_books_view AS
SELECT 
    b.id as book_id,
    b.name_book,
    a.first_name as author_first_name,
    a.last_name as author_last_name,
    bi.state,
    COUNT(*) as available_count
FROM public.book b
JOIN public.author a ON b.author = a.id
JOIN public.book_instance bi ON bi.book_information = b.id
WHERE bi.status = 'in stock'
GROUP BY b.id, b.name_book, a.first_name, a.last_name, bi.state
ORDER BY b.name_book, 
    CASE bi.state
        WHEN 'excellent' THEN 1
        WHEN 'good' THEN 2
        WHEN 'satisfactory' THEN 3
        WHEN 'dilapidated' THEN 4
        ELSE 5
    END;

-----------------------------------------------------------

CREATE OR REPLACE VIEW public.overdue_books_year_view AS
SELECT 
    r.id as reader_id,
    r.last_name AS reader_last_name,
    r.first_name AS reader_first_name,
    a.last_name AS author_last_name,
    a.first_name AS author_first_name,
    b.name_book AS book_title,
    i.date_time AS issue_date,
    i.date_return_expected,
    (CURRENT_DATE - i.date_time::DATE) as days_issued
FROM public.issuance i
JOIN public.reader r ON i.reader_id = r.id
JOIN public.book_instance bi ON i.book_id = bi.id
JOIN public.book b ON bi.book_information = b.id
JOIN public.author a ON b.author = a.id
WHERE i.date_return_fact IS NULL
AND i.date_time < CURRENT_DATE - INTERVAL '1 year'
ORDER BY i.date_time;

-----------------------------------------------------------

CREATE TABLE public.logs (
    id SERIAL PRIMARY KEY,
    log_date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    table_name VARCHAR(100) NOT NULL,
    log_content TEXT NOT NULL
);

-----------------------------------------------------------


CREATE OR REPLACE FUNCTION log_changes()
RETURNS TRIGGER AS $$
DECLARE
    v_operation TEXT;
    v_content TEXT;
BEGIN
    IF TG_OP = 'INSERT' THEN
        v_operation := 'INSERT';
        v_content := 'Добавлена запись: ' || row_to_json(NEW);
    ELSIF TG_OP = 'UPDATE' THEN
        v_operation := 'UPDATE';
        v_content := 'Изменена запись. Старые данные: ' || row_to_json(OLD) || 
                    ' Новые данные: ' || row_to_json(NEW);
    ELSIF TG_OP = 'DELETE' THEN
        v_operation := 'DELETE';
        v_content := 'Удалена запись: ' || row_to_json(OLD);
    END IF;
    
    INSERT INTO public.logs (table_name, log_content)
    VALUES (TG_TABLE_NAME, v_operation || ' - ' || v_content);
    
    IF TG_OP = 'DELETE' THEN
        RETURN OLD;
    ELSE
        RETURN NEW;
    END IF;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_log_author
    AFTER INSERT OR UPDATE OR DELETE ON public.author
    FOR EACH ROW EXECUTE FUNCTION log_changes();

CREATE TRIGGER trg_log_book
    AFTER INSERT OR UPDATE OR DELETE ON public.book
    FOR EACH ROW EXECUTE FUNCTION log_changes();

CREATE TRIGGER trg_log_reader
    AFTER INSERT OR UPDATE OR DELETE ON public.reader
    FOR EACH ROW EXECUTE FUNCTION log_changes();

CREATE TRIGGER trg_log_book_instance
    AFTER INSERT OR UPDATE OR DELETE ON public.book_instance
    FOR EACH ROW EXECUTE FUNCTION log_changes();

CREATE TRIGGER trg_log_issuance
    AFTER INSERT OR UPDATE OR DELETE ON public.issuance
    FOR EACH ROW EXECUTE FUNCTION log_changes();

CREATE TRIGGER trg_log_booking
    AFTER INSERT OR UPDATE OR DELETE ON public.booking
    FOR EACH ROW EXECUTE FUNCTION log_changes();

---------------------------------------------------------


-- Бронирование книги
SELECT reserve_book(4, 1);

-- Отмена бронирования
SELECT cancel_reservation(4, 1);

-- Выдача книги
SELECT issue_book(4, 1);

-- Получение информации о местоположениях книги
SELECT * FROM get_book_locations(1);

-- Просмотр доступных книг
SELECT * FROM available_books_view;

-- Просмотр долгосрочных просрочек
SELECT * FROM overdue_books_year_view;

-- Просмотр логов
SELECT * FROM logs ORDER BY log_date DESC;
