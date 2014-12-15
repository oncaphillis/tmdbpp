# tmdbpp README

## INTRO

A C++ API wrapper for the TMDB (The Movie Database) Web API. See
https://www.themoviedb.org/documentation/api.

See COPYING for Copyright

## Current Status 

Supports 

* search().company(query,page);
* search().movie(query,language,page);
* search().collection(query,page);
* search().keyword(query,page)
* search().tv(query,lang,page);
* search().tv().popular();
* search().tv().top_rated();
* search().tv().airing_today();
* search().tv().on_the_air();

* get().movie(id,lang)
* get().collection(id)
* get().company(id)
* get().keyword(id)
* get().tv(id,lang)

CMakefile and other configs are under developed-
  
