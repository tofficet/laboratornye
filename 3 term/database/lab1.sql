create database t01_library;

create table public.author(
id serial primary key,
first_name varchar(80) not null,
last_name varchar(80) not null
);

create table public.publishing_house(
id SERIAL primary key,
name varchar(80) not null,
city varchar(80) not null
);

create table public.book(
id serial primary key,
name_book varchar(80) not null,
author int references public.author(id),
publisher_code int references public.publishing_house(id),
edition int not null,
year_publication int,
circulation int
);

create type sex as enum ('male', 'female');

create table public.reader(
id serial primary key,
first_name varchar(80) not null,
last_name varchar(80) not null,
date_of_birth date,
gender sex,
date_of_regestration date
);


create type availability as enum ('in stock, issued, reserved');
create type state as enum ('excellent, good, satisfactory, dilapidated, lost');

create table public.book_instance(
id serial primary key,
book_information int references public.book(id),
state state, 
status availability,
location varchar(80) not null
);

create table public.issuance(
reader_id int references public.reader(id),
book_id int references public.book_instance(id),
date_time timestamp,
date_return_expected date not null,
date_return_fact date NULL
);

create table booking(
id serial primary key,
number_reader_ticket int references public.reader(id),
book_information int references public.book(id),
data_time timestamp,
min_state state
);


