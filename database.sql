--
-- PostgreSQL database dump
--

-- Dumped from database version 10.12 (Ubuntu 10.12-0ubuntu0.18.04.1)
-- Dumped by pg_dump version 10.12 (Ubuntu 10.12-0ubuntu0.18.04.1)

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: 
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


--
-- Name: erase_recipe(); Type: FUNCTION; Schema: public; Owner: postgres
--

CREATE FUNCTION public.erase_recipe() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN DELETE FROM recipies WHERE id in (SELECT recipe_id FROM ingredients_of_recipies WHERE ingredient_id = OLD.id);
RETURN OLD; END;
$$;


ALTER FUNCTION public.erase_recipe() OWNER TO postgres;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: categories; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.categories (
    id integer NOT NULL,
    name character varying(255) NOT NULL,
    photo bytea
);


ALTER TABLE public.categories OWNER TO postgres;

--
-- Name: categories_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.categories_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.categories_id_seq OWNER TO postgres;

--
-- Name: categories_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.categories_id_seq OWNED BY public.categories.id;


--
-- Name: ingredients; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.ingredients (
    id integer NOT NULL,
    name character varying(255) NOT NULL,
    meansurement character varying(255)
);


ALTER TABLE public.ingredients OWNER TO postgres;

--
-- Name: ingredients_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.ingredients_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.ingredients_id_seq OWNER TO postgres;

--
-- Name: ingredients_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.ingredients_id_seq OWNED BY public.ingredients.id;


--
-- Name: ingredients_of_recipies; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.ingredients_of_recipies (
    id integer NOT NULL,
    ingredient_id integer NOT NULL,
    recipe_id integer NOT NULL,
    count integer NOT NULL
);


ALTER TABLE public.ingredients_of_recipies OWNER TO postgres;

--
-- Name: ingredients_of_recipies_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.ingredients_of_recipies_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.ingredients_of_recipies_id_seq OWNER TO postgres;

--
-- Name: ingredients_of_recipies_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.ingredients_of_recipies_id_seq OWNED BY public.ingredients_of_recipies.id;


--
-- Name: recipe_photo; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.recipe_photo (
    id integer NOT NULL,
    photo bytea NOT NULL
);


ALTER TABLE public.recipe_photo OWNER TO postgres;

--
-- Name: recipies; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.recipies (
    id integer NOT NULL,
    category_id integer,
    algorithm text,
    chosen boolean
);


ALTER TABLE public.recipies OWNER TO postgres;

--
-- Name: recipies_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE public.recipies_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.recipies_id_seq OWNER TO postgres;

--
-- Name: recipies_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE public.recipies_id_seq OWNED BY public.recipies.id;


--
-- Name: categories id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.categories ALTER COLUMN id SET DEFAULT nextval('public.categories_id_seq'::regclass);


--
-- Name: ingredients id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.ingredients ALTER COLUMN id SET DEFAULT nextval('public.ingredients_id_seq'::regclass);


--
-- Name: ingredients_of_recipies id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.ingredients_of_recipies ALTER COLUMN id SET DEFAULT nextval('public.ingredients_of_recipies_id_seq'::regclass);


--
-- Name: recipies id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.recipies ALTER COLUMN id SET DEFAULT nextval('public.recipies_id_seq'::regclass);


--
-- Data for Name: categories; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.categories (id, name, photo) FROM stdin;
\.


--
-- Data for Name: ingredients; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.ingredients (id, name, meansurement) FROM stdin;
\.


--
-- Data for Name: ingredients_of_recipies; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.ingredients_of_recipies (id, ingredient_id, recipe_id, count) FROM stdin;
\.


--
-- Data for Name: recipe_photo; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.recipe_photo (id, photo) FROM stdin;
\.


--
-- Data for Name: recipies; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.recipies (id, category_id, algorithm, chosen) FROM stdin;
\.


--
-- Name: categories_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.categories_id_seq', 1, false);


--
-- Name: ingredients_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.ingredients_id_seq', 1, true);


--
-- Name: ingredients_of_recipies_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.ingredients_of_recipies_id_seq', 1, true);


--
-- Name: recipies_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('public.recipies_id_seq', 1, true);


--
-- Name: categories categories_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.categories
    ADD CONSTRAINT categories_pkey PRIMARY KEY (id);


--
-- Name: ingredients_of_recipies ingredients_of_recipies_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.ingredients_of_recipies
    ADD CONSTRAINT ingredients_of_recipies_pkey PRIMARY KEY (id);


--
-- Name: ingredients ingredients_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.ingredients
    ADD CONSTRAINT ingredients_pkey PRIMARY KEY (id);


--
-- Name: recipies recipies_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.recipies
    ADD CONSTRAINT recipies_pkey PRIMARY KEY (id);


--
-- Name: ingredients erase_recipe_trigger; Type: TRIGGER; Schema: public; Owner: postgres
--

CREATE TRIGGER erase_recipe_trigger BEFORE DELETE ON public.ingredients FOR EACH ROW EXECUTE PROCEDURE public.erase_recipe();


--
-- Name: ingredients_of_recipies ingredients_of_recipies_ingredient_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.ingredients_of_recipies
    ADD CONSTRAINT ingredients_of_recipies_ingredient_id_fkey FOREIGN KEY (ingredient_id) REFERENCES public.ingredients(id) ON DELETE CASCADE;


--
-- Name: ingredients_of_recipies ingredients_of_recipies_recipe_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.ingredients_of_recipies
    ADD CONSTRAINT ingredients_of_recipies_recipe_id_fkey FOREIGN KEY (recipe_id) REFERENCES public.recipies(id) ON DELETE CASCADE;


--
-- Name: recipe_photo recipe_photo_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.recipe_photo
    ADD CONSTRAINT recipe_photo_id_fkey FOREIGN KEY (id) REFERENCES public.recipies(id) ON DELETE CASCADE;


--
-- Name: recipies recipies_category_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.recipies
    ADD CONSTRAINT recipies_category_id_fkey FOREIGN KEY (category_id) REFERENCES public.categories(id) ON DELETE CASCADE;


--
-- PostgreSQL database dump complete
--

