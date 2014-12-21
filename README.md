# tmdbpp README

## INTRO

A C++ API wrapper for the TMDB (The Movie Database) Web API. See
https://www.themoviedb.org/documentation/api.

See COPYING for Copyright

## Current Status 

Supports 

* search().collection(query,page);
* search().company(query,page);
* search().keyword(query,page)
* search().movie(query,language,page);
* search().movie().credits(id,language);
* search().person(query,page,adult);
* search().person().movie_credits(id);
* search().person().tv_credits(id);
* search().person().combined_credits(id);
* search().tv(query,lang,page);
* search().tv().airing_today();
* search().tv().on_the_air();
* search().tv().popular();
* search().tv().top_rated();

* get().movie(id,lang)
* get().collection(id)
* get().company(id)
* get().keyword(id)
* get().tv(id,lang)
* get().person(id)

CMakefile and other configs are under developed-
  
