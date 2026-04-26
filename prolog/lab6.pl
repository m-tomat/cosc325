% male facts
male(brian).
male(matthew).
male(josiah).
male(tomjr).
male(tomsr).
male(richard).
male(richardjr).

% female facts
female(analise).
female(beverly).
female(anne).
female(june).
female(kristine).
female(sharon).

% parent(X, Y) means X is the parent of Y
parent(brian,josiah).
parent(brian,analise).
parent(kristine,josiah).
parent(kristine,analise).
parent(tomjr,brian).
parent(tomjr,matthew).
parent(tomsr,tomjr).
parent(richard,beverly).
parent(richard,richardjr).
parent(richard,sharon).
parent(beverly,brian).
parent(beverly,matthew).
parent(anne,tomjr).
parent(june,beverly).
parent(june,richardjr).
parent(june,sharon).

father(X, Y) :- male(X), parent(X, Y).
mother(X, Y) :- female(X), parent(X, Y).
child(X, Y) :- parent(Y, X).
son(X, Y) :- child(X, Y), male(X).
daughter(X, Y) :- child(X, Y), female(X).
grandparent(X, Y) :- parent(X, Z), parent(Z, Y).
grandfather(X, Y) :- grandparent(X, Y), male(X).
grandmother(X, Y) :- grandparent(X, Y), female(X).

sibling(X, Z) :- parent(P, X), parent(P, Z), X \= Z.

% aunt
aunt(X, Y) :- female(X), sibling(X, P), parent(P, Y).

% uncle
uncle(X, Y) :- male(X), sibling(X, P), parent(P, Y).

% reverse
reverse([], []).
reverse([H|T], R) :- reverse(T, RT), append(RT, [H], R).
