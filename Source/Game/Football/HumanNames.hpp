#pragma once
#include <array>
#include <string_view>
#include <optional>
#include "NationalAssociation.hpp"

namespace Names
{

	constexpr std::size_t NameCountByAssociation = 300;

	using NamesByAssociation = std::vector<std::string_view>;

	namespace FirstNames
	{
		constexpr std::array Belgium
		{
			"Mathieu", "Nicolas", "Alexandre", "Thomas", "Julien", "Maxime", "Benjamin", "Antoine", "Simon", "Arthur",
			"Lucas", "Quentin", "Louis", "Victor", "Robin", "Adrien", "Hugo", "Baptiste", "Félix", "Gabriel",
			"Martin", "Raphaël", "Tristan", "Guillaume", "Clément", "Thibault", "Charles", "Jérémy", "Romain", "Florian",
			"Axel", "Dylan", "Léo", "Eliott", "Noah", "Liam", "Ethan", "Nathan", "Logan", "Aaron",
			"Mohamed", "Adam", "Ibrahim", "Rayan", "Mehdi", "Yassine", "Amine", "Karim", "Samy", "Nassim",
			"Yanis", "Imran", "Ilyas", "Aymen", "Sami", "Reda", "Nolan", "Kylian", "Noam", "Lenny",
			"Timeo", "Mael", "Lorenzo", "Mathis", "Sacha", "Evan", "Enzo", "Nael", "Killian", "Naïm",
			"Jason", "Kevin", "Michael", "Steven", "Jonathan", "Anthony", "Jordan", "Diego", "Cédric", "Yannick",
			"Loïc", "Florent", "Gregory", "Laurent", "Sébastien", "Xavier", "Fabrice", "Olivier", "Vincent", "Jeremy",
			"Arnaud", "Christophe", "David", "Frédéric", "Philippe", "Stéphane", "Benoît", "Damien", "Gaëtan", "Gilles",
			"Jean", "Marc", "Pierre", "Samuel", "Thierry", "Yves", "Alain", "Bernard", "Daniel", "Eric",
			"François", "Georges", "Henri", "Jacques", "Jean-Luc", "Michel", "Patrick", "Paul", "Robert", "Serge",
			"André", "Bruno", "Charles-Henri", "Didier", "Emmanuel", "Frank", "Guy", "Hervé", "Jean-Marc", "Luc",
			"Mario", "Nicolas", "Olivier", "Pascal", "Régis", "Stéphane", "Thomas", "Vincent", "Willy", "Yannick",
			"Zacharie", "Achille", "Bastien", "Corentin", "Dorian", "Edgar", "Fabien", "Gaspard", "Hippolyte", "Ismaël",
			"Jules", "Kenzo", "Léandre", "Marceau", "Nino", "Oscar", "Pablo", "Quentin", "Rafael", "Soren",
			"Tiago", "Ulysse", "Vadim", "Wandrille", "Xander", "Ylan", "Zachary", "Aaron", "Bryan", "Cameron",
			"Diego", "Eden", "Finn", "Gael", "Hector", "Ilan", "Jaden", "Kian", "Liam", "Mateo",
			"Nolan", "Owen", "Pablo", "Quentin", "Raphael", "Soren", "Theo", "Ugo", "Victor", "Wesley",
			"Xavier", "Yael", "Zack", "Aiden", "Bradley", "Caleb", "Dylan", "Elijah", "Felix", "Gabriel",
			"Hudson", "Isaac", "Jasper", "Kai", "Leo", "Mason", "Nathan", "Oliver", "Parker", "Quinn",
			"Riley", "Sawyer", "Theodore", "Uriel", "Vincent", "William", "Xander", "Yusuf", "Zane", "Aaron",
			"Braden", "Carter", "Derek", "Eli", "Finley", "Grant", "Harrison", "Ian", "Jaxon", "Kellan",
			"Landon", "Miles", "Noah", "Owen", "Preston", "Quentin", "Ryan", "Spencer", "Tanner", "Upton",
			"Vaughn", "Wesley", "Xavier", "Yates", "Zachary", "Abram", "Benson", "Caspian", "Dashiell", "Eamon",
			"Finnian", "Gideon", "Hayes", "Isaiah", "Judah", "Kieran", "Lincoln", "Magnus", "Nolan", "Orson",
			"Phineas", "Quincy", "Reuben", "Soren", "Tate", "Upton", "Vance", "Walter", "Xander", "Yates",
			"Zephaniah", "Alistair", "Beckett", "Caius", "Declan", "Eamon", "Fitzgerald", "Gatlin", "Hayden", "Ivan",
			"Julian", "Kian", "Lachlan", "Maddox", "Nolan", "Orion", "Pierce", "Quade", "Remy", "Sawyer",
			"Tiernan", "Uilleam", "Vaughn", "Winston", "Xeno", "Yorick", "Zephyr", "Ambrose", "Benedict", "Cormac"
		};

		constexpr std::array England
		{
			"James", "John", "Robert", "Michael", "William", "David", "Richard", "Joseph", "Thomas", "Charles",
			"Christopher", "Daniel", "Matthew", "Anthony", "Mark", "Donald", "Steven", "Paul", "Andrew", "Joshua",
			"Kenneth", "Kevin", "Brian", "George", "Edward", "Ronald", "Timothy", "Jason", "Jeffrey", "Ryan",
			"Jacob", "Gary", "Nicholas", "Eric", "Jonathan", "Stephen", "Larry", "Justin", "Scott", "Brandon",
			"Benjamin", "Samuel", "Gregory", "Frank", "Alexander", "Raymond", "Patrick", "Jack", "Dennis", "Jerry",
			"Tyler", "Aaron", "Jose", "Adam", "Henry", "Nathan", "Douglas", "Zachary", "Peter", "Kyle",
			"Walter", "Ethan", "Jeremy", "Harold", "Keith", "Christian", "Roger", "Noah", "Gerald", "Carl",
			"Terry", "Sean", "Austin", "Arthur", "Lawrence", "Jesse", "Dylan", "Bryan", "Joe", "Jordan",
			"Billy", "Bruce", "Gabriel", "Logan", "Albert", "Willie", "Alan", "Juan", "Wayne", "Elijah",
			"Randy", "Roy", "Vincent", "Ralph", "Eugene", "Russell", "Bobby", "Mason", "Philip", "Louis",
			"Harry", "Fred", "Howard", "Owen", "Dale", "Reginald", "Alex", "Jamie", "Leonard", "Caleb",
			"Nathaniel", "Theodore", "Manuel", "Luis", "Ruben", "Vernon", "Damian", "Mitchell", "Edwin", "Don",
			"Cameron", "Marlon", "Derek", "Warren", "Darrell", "Javier", "Rudy", "Marcus", "Mario", "Leroy",
			"Micah", "Dominic", "Leon", "Dustin", "Kurt", "Travis", "Bradley", "Allan", "Karl", "Neil",
			"Geoffrey", "Gordon", "Jeff", "Frederick", "Trevor", "Glenn", "Evan", "Terrance", "Miguel", "Julian",
			"Stanley", "Jesus", "Eddie", "Gene", "Marc", "Todd", "Calvin", "Lee", "Kristopher", "Edgar",
			"Fernando", "Malcolm", "Oscar", "Ben", "Curtis", "Melvin", "Erik", "Lorenzo", "Desmond", "Jay",
			"Mathew", "Shawn", "Tony", "Luke", "Tom", "Brent", "Ramon", "Martin", "Craig", "Jim",
			"Jean", "Shane", "Derrick", "Dean", "Jon", "Ricardo", "Miles", "Jared", "Gilbert", "Clarence",
			"Nicolas", "Eli", "Marvin", "Leland", "Hector", "Victor", "Carlos", "Rafael", "Ian", "Cory",
			"Angel", "Daryl", "Ross", "Kelly", "Andre", "Tyler", "Byron", "Julius", "Dwight", "Spencer",
			"Erick", "Rodney", "Brett", "Brendan", "Dane", "Seth", "Casey", "Clifford", "Garrett", "Jerome",
			"Drew", "Felix", "Jimmy", "Emanuel", "Ivan", "Johnny", "Elliott", "Stuart", "Fredrick", "Darren",
			"Isiah", "Blake", "Raul", "Omar", "Wade", "Declan", "Kane", "Grant", "Theo", "Xavier",
			"Hugh", "Jude", "Riley", "Emmett", "Maxwell", "Kai", "Forrest", "Morgan", "Roman", "Reed",
			"Sawyer", "Dax", "Otis", "Ari", "Amir", "Loren", "Judah", "Brayden", "Chase", "Abel",
			"Kian", "Rhys", "Jasiah", "Kaison", "Franklin", "Iker", "Cason", "Yusuf", "Matteo", "Porter",
			"Kyrie", "Julius", "Davis", "Nasir", "Kellen", "Moshe", "Armani", "Kaysen", "Soren", "Kamari",
			"Colson", "Zayn", "Koa", "Prince", "Dash", "Rio", "Shiloh", "Jaxx", "Zaid", "Ameer",
			"Duke", "Quentin", "Enoch", "Kashton", "Jakari", "Reece", "Cairo", "Shepherd", "Bodie", "Magnus",
		};


		constexpr std::array France
		{
			"Pierre", "Jean", "Paul", "Jacques", "Robert", "Michel", "François", "André", "René", "Louis",
			"Alain", "Gérard", "Patrick", "Philippe", "Bernard", "Christian", "Daniel", "Thierry", "Eric", "Emmanuel",
			"Franck", "Stéphane", "Laurent", "Pascal", "Sébastien", "Nicolas", "David", "Julien", "Antoine", "Marc",
			"Yves", "Olivier", "Guillaume", "Vincent", "Mathieu", "Luc", "Thomas", "Christophe", "Bruno", "Hugues",
			"Cédric", "Fabrice", "Arnaud", "Jérôme", "Ludovic", "Romain", "Grégory", "Alexandre", "Sylvain", "Manuel",
			"Raphaël", "Aurélien", "Adrien", "Loïc", "Kévin", "Benoît", "Damien", "Gilles", "Gaël", "Mickaël",
			"Fabien", "Jérémy", "Yann", "Rémi", "Matthieu", "Maxime", "Jordan", "Alexis", "Mehdi", "Karim",
			"Nassim", "Yassine", "Samir", "Fouad", "Hakim", "Youssef", "Amine", "Ibrahim", "Rayan", "Imran",
			"Yanis", "Nolan", "Noah", "Liam", "Ethan", "Gabriel", "Nathan", "Dylan", "Enzo", "Lucas",
			"Hugo", "Léo", "Timeo", "Aaron", "Eliott", "Sacha", "Maël", "Noam", "Evan", "Tom",
			"Clément", "Théo", "Arthur", "Martin", "Quentin", "Bastien", "Valentin", "Corentin", "Florian", "Morgan",
			"Kylian", "Yan", "Mathis", "Lorenzo", "Tiago", "Diego", "Cameron", "Eden", "Nino", "Pablo",
			"Rafael", "Tony", "Samuel", "Victor", "Alexandre", "Maxence", "Robin", "Tristan", "Quentin", "Jules",
			"Augustin", "Marceau", "Achille", "Gaspard", "Balthazar", "Hippolyte", "Célestin", "Basile", "Ernest", "Félix",
			"Edgar", "Rodolphe", "Aristide", "Lazare", "Honoré", "Anatole", "Isidore", "Cyprien", "Prosper", "Irénée",
			"Hilaire", "Firmin", "Gatien", "Léonce", "Séraphin", "Ambroise", "Barnabé", "Côme", "Donatien", "Evariste",
			"Fulgence", "Gérald", "Hervé", "Ignace", "Juvénal", "Léandre", "Marcellin", "Nestor", "Onésime", "Pacôme",
			"Rémy", "Siméon", "Thibaut", "Urbain", "Vespasien", "Wenceslas", "Xavier", "Yvon", "Zacharie", "Aimé",
			"Benjamin", "Clovis", "Désiré", "Elouan", "Fabian", "Gauthier", "Hadrien", "Ismaël", "Josué", "Killian",
			"Lancelot", "Marin", "Nathanaël", "Oscar", "Priam", "Quentin", "Roméo", "Sullivan", "Titouan", "Ulysse",
			"Virgile", "Wandrille", "Xander", "Yoann", "Zachary", "Aloïs", "Briac", "Cyprien", "Dorian", "Esteban",
			"Félix", "Gabin", "Hadrien", "Iban", "Johan", "Kenzo", "Lorenzo", "Mathys", "Nino", "Owen",
			"Pablo", "Quentin", "Raphaël", "Sasha", "Tao", "Ugo", "Vadim", "Wassim", "Xavier", "Ylan",
			"Zack", "Aaron", "Bastien", "Cameron", "Diego", "Eden", "Finn", "Gabriel", "Hector", "Ilan",
			"Jaden", "Kian", "Liam", "Mateo", "Nolan", "Owen", "Pablo", "Quentin", "Raphael", "Soren",
			"Theo", "Ugo", "Victor", "Wesley", "Xavier", "Yael", "Zack", "Aiden", "Bradley", "Caleb"
		};

		constexpr std::array Germany
		{
			"Lukas", "Maximilian", "Paul", "Jonas", "Leon", "Finn", "Elias", "Ben", "Luis", "Noah",
			"Felix", "Tim", "Luca", "Niklas", "Jan", "Moritz", "David", "Julian", "Henry", "Philipp",
			"Alexander", "Jakob", "Max", "Tobias", "Simon", "Daniel", "Fabian", "Marvin", "Jannik", "Tom",
			"Nico", "Marc", "Michael", "Leo", "Emil", "Oskar", "Anton", "Mats", "Linus", "Jonathan",
			"Lennard", "Adrian", "Kevin", "Marius", "Lennox", "Milan", "Theo", "Matteo", "Liam", "Nils",
			"Florian", "Mika", "Hannes", "Levin", "Marlon", "Jason", "Maurice", "Leonard", "Rafael", "Colin",
			"Justin", "Malte", "Bennet", "Cedric", "Tristan", "Till", "Pepe", "Mattis", "Julius", "Nick",
			"Kilian", "Joel", "Lasse", "Mio", "Jannis", "Lenny", "Lars", "Fritz", "Johannes", "Dennis",
			"Benedikt", "Erik", "Konstantin", "Leander", "Magnus", "Nikolai", "Roman", "Vincent", "Yannik", "Aaron",
			"Bastian", "Carl", "Darian", "Elia", "Fabio", "Gianluca", "Henrik", "Ian", "Jamie", "Kian",
			"Lorenz", "Mateo", "Nathan", "Ole", "Pascal", "Quentin", "Ricardo", "Sebastian", "Timo", "Umut",
			"Valentin", "Wilhelm", "Xaver", "Yannic", "Zacharias", "Albert", "Bela", "Clemens", "Diego", "Emilio",
			"Falk", "Gustav", "Hugo", "Ivo", "Jasper", "Kai", "Leif", "Mats", "Nino", "Oliver",
			"Pablo", "Quirin", "Rasmus", "Sven", "Titus", "Uwe", "Vito", "Wenzel", "Xander", "Yves",
			"Zeno", "Björn", "Carlos", "Darius", "Emanuel", "Finnley", "Gero", "Hauke", "Igor",
			"Janosch", "Keno", "Leandro", "Manuel", "Nathaniel", "Otis", "Phil", "Quintus", "René", "Silas",
			"Tammo", "Ulrich", "Veit", "Wieland", "Xenon", "Yannis", "Zeus", "Achim", "Benedict", "Cornelius",
			"Dieter", "Eberhard", "Ferdinand", "Gerhard", "Hartmut", "Ingo", "Jörn", "Klaus", "Lothar", "Manfred",
			"Norbert", "Otto", "Peter", "Quirinus", "Rainer", "Stefan", "Torben", "Udo", "Volker", "Walter",
			"Xerxes", "Yorick", "Zoltan", "Achmed", "Bernhard", "Christoph", "Dietmar", "Eckhard", "Falko", "Gernot",
			"Hartwig", "Ingolf", "Jürgen", "Knut", "Ludwig", "Marcus", "Olaf", "Paulus", "Quint",
			"Ralf", "Siegfried", "Torsten", "Ulf", "Volkmar", "Werner", "Xenophon", "Yannick", "Adalbert",
			"Bernd", "Christian", "Detlef", "Egon", "Felix", "Gerald", "Harald", "Ingmar", "Jens", "Kurt",
			"Ludger", "Matthias", "Nikolaus", "Patrick", "Quintin", "Rüdiger", "Siegmund", "Thorsten",
			"Volkhard", "Wilfried", "Xaverius", "Yves", "Zachary", "Ludolf", "Adelbert", "Bertram", "Christof", "Diethelm", "Ewald",
			"Felician", "Gerolf", "Harro", "Ingemar", "Jörg", "Kuno", "Matthäus", "Niklas", "Dietrich", "Gerhardt",
			"Patrizius", "Rüdigar", "Siegbert", "Thoralf", "Volkhart", "Xenophanes", "Yvo", "Siegward",
			"Zachäus", "Volkwin", "Ingobert"
		};

		constexpr std::array Italy
		{
			"Alessandro", "Marco", "Giovanni", "Andrea", "Luca", "Matteo", "Francesco", "Simone", "Lorenzo", "Mattia",
			"Riccardo", "Federico", "Davide", "Antonio", "Giuseppe", "Stefano", "Nicola", "Paolo", "Michele", "Daniele",
			"Emanuele", "Tommaso", "Gabriele", "Filippo", "Roberto", "Alberto", "Enrico", "Leonardo", "Dario", "Christian",
			"Fabio", "Massimo", "Edoardo", "Giorgio", "Mario", "Salvatore", "Vincenzo", "Angelo", "Diego", "Mirko",
			"Samuele", "Ivan", "Luciano", "Maurizio", "Sergio", "Claudio", "Guido", "Umberto", "Valerio", "Bruno",
			"Raffaele", "Pietro", "Carlo", "Elia", "Omar", "Nicolò", "Alessio", "Manuel", "Carmine", "Vittorio",
			"Armando", "Fabrizio", "Giacomo", "Ruggero", "Sandro", "Tiziano", "Ugo", "Valentino", "Walter", "Yuri",
			"Adriano", "Beppe", "Cesare", "Dante", "Emiliano", "Furio", "Gennaro", "Igor", "Lino", "Marcello",
			"Nino", "Oreste", "Piero", "Remo", "Silvio", "Tullio", "Ubaldo", "Vasco", "Zeno", "Achille",
			"Benito", "Ciro", "Dino", "Ezio", "Ferdinando", "Gaspare", "Ignazio", "Leone", "Mauro", "Natale",
			"Ottavio", "Pasquale", "Renato", "Silvano", "Tarcisio", "Ulisse", "Virgilio", "Wilfredo", "Yago", "Zaccaria",
			"Aldo", "Biagio", "Corrado", "Domenico", "Ermanno", "Fausto", "Gerardo", "Ippolito", "Lamberto", "Marino",
			"Nereo", "Osvaldo", "Primo", "Rinaldo", "Sabatino", "Taddeo", "Ulderico", "Valfredo", "Zaira", "Adamo",
			"Bartolomeo", "Clemente", "Donato", "Egidio", "Fabiano", "Gualtiero", "Iacopo", "Lazzaro", "Marzio", "Nazzareno",
			"Olimpio", "Palmiro", "Rodolfo", "Saverio", "Tancredi", "Urbano", "Vilfredo", "Wladimiro", "Yorick", "Zefiro",
			"Adelmo", "Baldassarre", "Celestino", "Demetrio", "Eustachio", "Fortunato", "Gaudenzio", "Iginio", "Ludovico", "Macario",
			"Norberto", "Ottorino", "Pancrazio", "Romeo", "Sebastiano", "Tazio", "Ugoberto", "Vindice", "Zaccaria", "Abele",
			"Benedetto", "Cipriano", "Dionigi", "Evaristo", "Feliciano", "Gelasio", "Ilario", "Lorenzino", "Muzio", "Nestore",
			"Ottone", "Pompeo", "Romolo", "Sabino", "Taziano", "Ubaldo", "Virginio", "Waltero", "Yago", "Zanobi",
			"Adamo", "Bartolo", "Celso", "Diodato", "Ermete", "Fabriziano", "Gaspare", "Ildebrando", "Lazzaro", "Marziano",
			"Nazzareno", "Olimpio", "Palmiero", "Rodolfo", "Saveriano", "Tancredo", "Urbino", "Vilfredo", "Wladimir", "Yorik",
			"Zefirino", "Adelchi", "Baldovino", "Celestino", "Demetrio", "Eustachio", "Fortunato", "Gaudenzio", "Iginio", "Ludovico"
		};

		constexpr std::array Netherlands
		{
			"Johan", "Piet", "Jan", "Klaas", "Willem", "Gerrit", "Hendrik", "Dirk", "Maarten", "Adriaan",
			"Cornelis", "Jacob", "Herman", "Pieter", "Theo", "Bram", "Sander", "Robert", "Erik", "Thomas",
			"Frank", "Ronald", "Marcel", "Patrick", "Dennis", "Michael", "Richard", "Martin", "Paul", "Mark",
			"Steven", "Bas", "Jeroen", "Ruud", "Vincent", "Daan", "Lucas", "Tom", "Tim", "Niels",
			"Robin", "Milan", "Jasper", "Luuk", "Sem", "Finn", "Levi", "Stijn", "Jayden", "Jesse",
			"Damian", "Quinn", "Mats", "Guus", "Thijs", "Mehmet", "Mohamed", "Ali", "Hüseyin", "Ahmet",
			"Mustafa", "Yusuf", "Ömer", "Ismail", "Emre", "Eren", "Kerem", "Furkan", "Baran", "Can",
			"Arda", "Efe", "Kaan", "Mert", "Yunus", "Berat", "Cem", "Enes", "Tarik", "Burak",
			"Hasan", "Ibrahim", "Osman", "Selim", "Yasin", "Adem", "Erdem", "Kadir", "Murat", "Özgür",
			"Taylan", "Umut", "Yigit", "Aydin", "Cihan", "Deniz", "Eray", "Fatih", "Gökhan", "Hakan",
			"Ilhan", "Kemal", "Mehmet Ali", "Mete", "Nuri", "Orhan", "Sinan", "Tolga", "Tunç", "Yavuz",
			"Aaron", "Benjamin", "Daniel", "Elias", "Gabriel", "Isaac", "Joshua", "Noah", "Samuel", "David",
			"Elijah", "Joseph", "Levi", "Moses", "Asher", "Ethan", "Gideon", "Jacob", "Judah", "Reuben",
			"Simeon", "Zachary", "Caleb", "Enoch", "Jabez", "Jedidiah", "Jeremiah", "Malachi", "Micah", "Nathan",
			"Abel", "Adam", "Eve", "Noah", "Abraham", "Isaac", "Jacob", "Joseph", "Aaron", "Moses",
			"Samuel", "David", "Solomon", "Elijah", "Elisha", "Jonah", "Daniel", "Ezekiel", "Hosea", "Joel",
			"Amos", "Obadiah", "Jonah", "Micah", "Nahum", "Habakkuk", "Zephaniah", "Haggai", "Zechariah", "Malachi",
			"Matthew", "Mark", "Luke", "John", "Peter", "Paul", "James", "Andrew", "Philip", "Bartholomew",
			"Thomas", "Matthew", "James", "Thaddaeus", "Simon", "Judas", "Matthias", "Stephen", "Philip", "Prochorus",
			"Nicanor", "Timon", "Parmenas", "Nicolas", "Barnabas", "Silas", "Timothy", "Titus", "Philemon", "Onesimus",
			"Sosthenes", "Apollos", "Epaphroditus", "Caesar", "Demetrius", "Tychicus", "Trophimus", "Aristarchus", "Crispus", "Gaius",
			"Sopater", "Secundus", "Epenetus", "Andronicus", "Stachys", "Ampliatus", "Urbanus", "Narcissus", "Apelles", "Lucius",
			"Jason", "Sosipater", "Tertius", "Erastus", "Quartus", "Euodia", "Syntyche", "Clement", "Phlegon", "Hermes",
			"Asyncritus", "Phlegon", "Hermas", "Patrobas", "Hermes", "Linus", "Claudia", "Eubulus", "Pudens", "Trophimus",
			"Persis", "Stachys", "Rufus", "Asyncritus", "Phlegon", "Hermes", "Patrobas", "Hermas", "Philologus", "Julia",
			"Nereus", "Olympas", "Tryphena", "Tryphosa", "Persis", "Rufus", "Asyncritus", "Phlegon", "Hermes", "Patrobas"
		};

		constexpr std::array Nigeria
		{
			"Abdul", "Abdullahi", "Abdulazeez", "Abimbola", "Abiodun", "Abiola", "Abubakar", "Adebayo", "Adebisi", "Adebola",
			"Ade", "Adedayo", "Adefolake", "Ademola", "Adetayo", "Adetokunbo", "Adewale", "Afolabi", "Akin", "Akinleye",
			"Akinlolu", "Akintoye", "Akinwale", "Alaba", "Amarachi", "Amina", "Amir", "Ayodeji", "Ayodele", "Ayomide",
			"Ayotunde", "Babajide", "Babatunde", "Bamidele", "Bashir", "Bashiru", "Benjamin", "Bolanle", "Bolaji", "Bukola",
			"Chibuzo", "Chidiebere", "Chijioke", "Chike", "Chikadibia", "Chikodi", "Chima", "Chinedu", "Chinonso", "Chizor",
			"Chukwudi", "Chukwuka", "Chukwuma", "Damilola", "Damola", "Dare", "Darasimi", "David", "Duro", "Durojaiye",
			"Durojaye", "Durotoye", "Ebuka", "Ekene", "Eliud", "Emeka", "Emmanuel", "Eniola", "Fatai", "Femi",
			"Feyisayo", "Fola", "Folake", "Folarin", "Ganiyu", "Gbenga", "Gbolahan", "Goke", "Hakeem", "Hassan",
			"Idris", "Ifeanyi", "Ifeanyichukwu", "Ifeoluwa", "Ifedayo", "Ikenna", "Iliya", "Ibrahim", "Ismail", "Ishola",
			"Jibril", "Jibola", "Jide", "Joshua", "Jude", "Juwon", "Kazeem", "Kehinde", "Khalid", "Kola",
			"Kunle", "Ladipo", "Lekan", "Michael", "Modupe", "Moses", "Murtala", "Musa", "Niyi", "Nnamdi",
			"Ola", "Oladapo", "Oladipo", "Olanrewaju", "Olamide", "Olamilekan", "Olaniyi", "Olajide", "Olajuwon", "Olakunle",
			"Olalekan", "Olumide", "Olumuyiwa", "Olusegun", "Olutayo", "Olu", "Oluwadamilola", "Oluwafemi", "Oluwakemi", "Oluwasegun",
			"Oluwaseun", "Oluwaseyi", "Oluwatamilore", "Oluwatimilehin", "Oluwatobi", "Opeyemi", "Oreoluwa", "Osagie",
			"Raji", "Rasak", "Rauf", "Rilwan", "Rasaq", "Samuel", "Segun", "Seyi", "Sulaimon", "Suleiman",
			"Taiwo", "Tade", "Tajudeen", "Tariq", "Tarique", "Temidayo", "Temiloluwa", "Titi", "Titiola", "Tobi", "Tobiloba",
			"Tola", "Tolu", "Toluwa", "Toluwani", "Toyin", "Tunde", "Tunji", "Tunrayo", "Uche", "Uchechukwu",
			"Ugochukwu", "Umar", "Victor", "Wasiu", "Yemi", "Yusuf", "Zainab", "Zubair", "Zubairu", "Abayomi",
			"Adekunle", "Adesina", "Ajibola", "Akindele", "Bankole", "Daramola", "Ezekiel", "Gbadebo", "Ikenna", "Nduka",
			"Obinna", "Obiora", "Olufemi", "Olajumoke", "Olorunfemi", "Oluwatosin", "Remilekun", "Salau", "Shola", "Simeon",
			"Sotunde", "Sunday", "Timileyin", "Tochukwu", "Uchenna", "Uzoma", "Yaw", "Zacheus", "Zayyad", "Adegoke",
			"Adejare", "Adejobi", "Adejumo", "Adekola", "Adelaja", "Adelani", "Ademiluyi", "Adeniran", "Adeniyi", "Aderinsola",
			"Aderonke", "Adesanya", "Adesoji", "Adetola", "Adetunji", "Adigun", "Agboola", "Ajewole", "Akinbola", "Akinbode",
			"Akinfenwa", "Akingbade", "Akinsanya", "Akinyemi", "Alade", "Anjorin", "Anibaba", "Aremu", "Asade", "Ayinde",
			"Badejo", "Balogun", "Bamgbose", "Bamimore", "Banjo", "Bello", "Chukwuemeka", "Chukwunonso", "Dada", "Elebute",
			"Eze", "Ezechi", "Ezekwe", "Ezequiel", "Fagbenro", "Fajemirokun", "Fasanya", "Fatunmbi", "Gbadamosi", "Ibukun",
			"Ijagbemi", "Ilori", "Isiba", "Itua", "Jegede", "Jimoh", "Kasali", "Kolapo", "Majekodunmi", "Makinde",
			"Makanjuola", "Mayowa", "Mogaji", "Mohammed", "Morenikeji", "Nwankwo", "Nwachukwu", "Nwaeze", "Obafemi", "Odunayo", "Ogundare",
			"Ogunleye", "Ojo", "Okafor", "Okeke", "Olaniran", "Olayemi", "Olayinka", "Olufunmilayo", "Olumorin", "Olusanya",
			"Omoniyi", "Omotayo", "Onifade", "Onikoyi", "Orimolade", "Oyelowo", "Adebiyi", "Babajimi", "Oloruntoyin", "Tanimola"
		};

		constexpr std::array Portugal
		{
			"António", "José", "Manuel", "João", "Francisco", "Carlos", "Pedro", "Paulo", "Luís", "Jorge",
			"Ricardo", "Fernando", "Sérgio", "André", "Bruno", "Filipe", "Tiago", "Rafael", "Miguel", "Nuno",
			"Rui", "Marco", "Alexandre", "Daniel", "David", "Gonçalo", "Hugo", "Diogo", "Guilherme", "Mário",
			"Vítor", "Raul", "Samuel", "Gabriel", "Martim", "Simão", "Afonso", "Bernardo", "Tomás", "Rodrigo",
			"Salvador", "Vicente", "Vasco", "Leandro", "Ivo", "Mauro", "Edgar", "Emanuel", "Fabio", "Gustavo",
			"Henrique", "Ismael", "Jacinto", "Joaquim", "Jonas", "Leonardo", "Lucas", "Marcos", "Matias", "Mateus",
			"Maurício", "Nelson", "Nélson", "Nicolau", "Octávio", "Óscar", "Patrício", "Quirino", "Renato", "Roberto",
			"Rúben", "Salvador", "Sebastião", "Tadeu", "Telmo", "Teodoro", "Tiago", "Tobias", "Uriel", "Valter",
			"Xavier", "Zacarias", "Adriano", "Alberto", "Amadeu", "Amaro", "Américo", "Anselmo", "António", "Armando",
			"Artur", "Augusto", "Baltazar", "Benjamim", "Caetano", "Camilo", "César", "Cláudio", "Constantino", "Cristiano",
			"Dário", "Demétrio", "Dinis", "Domingos", "Duarte", "Edmundo", "Eduardo", "Eliseu", "Élio", "Emílio",
			"Estêvão", "Eugénio", "Eurico", "Fausto", "Fábio", "Feliciano", "Félix", "Firmino", "Flávio", "Florêncio",
			"Francisco", "Frederico", "Gabriel", "Gaspar", "Gil", "Gonçalo", "Guilherme", "Gustavo", "Helder", "Henrique",
			"Hermano", "Humberto", "Igor", "Inácio", "Isaías", "Isidro", "Jacinto", "Jaime", "Jerónimo", "Joaquim",
			"Joel", "Jonatas", "Jónatas", "Jorge", "José", "Júlio", "Júnior", "Justino", "Laurindo", "Leandro",
			"Leónidas", "Leonel", "Leopoldo", "Lourenço", "Lucas", "Luís", "Lúcio", "Márcio", "Marcelo", "Mário",
			"Martinho", "Martim", "Matias", "Maurício", "Mauro", "Maximiliano", "Melchior", "Micael", "Miguel", "Milton",
			"Moisés", "Narciso", "Natanael", "Nélson", "Nicolau", "Noé", "Norberto", "Octávio", "Olavo", "Óscar",
			"Osvaldo", "Otávio", "Patrício", "Paulo", "Pedro", "Plácido", "Quintino", "Rafael", "Raimundo", "Raul",
			"Régis", "Renato", "Ricardo", "Roberto", "Rodrigo", "Rómulo", "Rui", "Rúben", "Salvador", "Samuel",
			"Sandro", "Santiago", "Saul", "Sebastião", "Sérgio", "Silvestre", "Simão", "Sílvio", "Tadeu", "Teixeira",
			"Teodoro", "Tiago", "Timóteo", "Tobias", "Tomás", "Ubaldo", "Ulisses", "Uriel", "Valério", "Valter",
			"Vasco", "Vicente", "Vítor", "Xavier", "Zacarias", "Abel", "Abílio", "Abraão", "Adalberto", "Adelino",
			"Ademar", "Adolfo", "Adriano", "Afonso", "Agostinho", "Agostinho", "Aitor", "Alberto", "Alexandre", "Alfredo",
			"Alípio", "Almiro", "Altino", "Amadeu", "Amaro", "Américo", "Amílcar", "Anacleto", "André", "Ângelo",
			"Aníbal", "Anselmo", "António", "Antunes", "Aquiles", "Arlindo", "Armando", "Arménio", "Arnaldo", "Artur",
			"Augusto", "Aureliano", "Aurelio", "Aurélio", "Baltazar", "Bartolomeu", "Basílio", "Belmiro", "Benedito",
			"Benjamim", "Bernardino", "Bernardino", "Bernardo", "Boaventura", "Bonifácio", "Bráulio", "Brás", "Bruno",
			"Caetano", "Calisto", "Camilo", "Carlos", "Cássio", "Castro", "César", "Cipriano", "Cláudio", "Claro",
			"Claudio", "Cláudio", "Clóvis", "Constantino", "Cristian", "Cristóvão", "Custódio", "Damião", "Daniel",
			"Dário", "David", "Davi"
		};

		constexpr std::array Spain
		{
			"José", "Manuel", "Juan", "Francisco", "David", "Carlos", "Javier", "Daniel", "Luis", "Antonio",
			"Miguel", "Pedro", "Alejandro", "Jorge", "Sergio", "Pablo", "Fernando", "Rafael", "Raúl", "Diego",
			"Ángel", "Mario", "Eduardo", "Jesús", "Ricardo", "Alberto", "Roberto", "Enrique", "Alfredo", "Félix",
			"Guillermo", "Víctor", "Andrés", "Óscar", "Joaquín", "Emilio", "Gabriel", "Santiago", "Martín", "Salvador",
			"Marcos", "Iván", "Hugo", "Adrián", "Julián", "Álvaro", "Rubén", "Mateo", "Samuel", "Nicolás",
			"Lucas", "Aarón", "Ismael", "Saúl", "Esteban", "Leonardo", "Gonzalo", "Ignacio", "Tomás",
			"Federico", "Rodrigo", "Mauricio", "Armando", "Emanuel", "Cristian", "Sebastián", "Mariano", "Agustín", "Fabián",
			"Bernardo", "Ramiro", "Arturo", "Damián", "Elías", "Benjamín", "Julio", "César", "Héctor", "Gustavo",
			"Alonso", "Bruno", "Francisco Javier", "Teodoro", "Valentín", "Jerónimo", "Matías", "Jairo", "Santi", "Yael",
			"Aitor", "Iker", "Unai", "Eneko", "Koldo", "Gorka", "Aimar", "Iñaki", "Markel", "Xabier",
			"Jon", "Imanol", "Gaizka", "Asier", "Mikel", "Bingen", "Endika", "Urtzi", "Peru", "Kepa",
			"Egoitz", "Aritz", "Igor", "Julen", "Hodei", "Ibai", "Unax", "Oier", "Beñat", "Ander",
			"Ekaitz", "Gari", "Xabi", "Iñigo", "Urko", "Jokin", "Bittor", "Edorta", "Gorka", "Ibán",
			"Aimar", "Fran", "Josep", "Marco", "Bryan", "Javi", "Marc", "Kike", "Sergi", "Sol", "Mino",
			"Claudio", "Arsenio", "Ysai", "Valeno", "Blas", "Esmael", "Luciano", "Quique", "Estanislau",
			"Humberto", "Baldomero", "Carlito", "Aurelio", "Americo", "Ceferino", "Cristóbal", "Fidel",
			"Corsino", "Amado", "Adan", "Flavio", "Hiro", "Isidoro", "Joel", "Juan José", "Felipe", "Fabio",
			"Eugenio", "Eladio", "Beto", "José Carlos", "Enzo", "Carlo", "Carlos Alberto", "Carlos Eduardo",
			"Ginés", "Rolando", "Manu", "Severino", "Isaac", "Roque", "Osvaldo", "Germán", "Jesus", "Jaime",
		};

		constexpr std::array Sweden
		{
			"Liam", "William", "Lucas", "Elias", "Alexander", "Oliver", "Hugo", "Oscar", "Leo", "Axel",
			"Noah", "Theo", "Charlie", "Viktor", "Elliot", "Adam", "Anton", "Filip", "Edvin", "Gustav",
			"Emil", "Arvid", "Melvin", "Albin", "Vincent", "Alfred", "Sixten", "Max", "Erik", "Isak",
			"Benjamin", "Milton", "Valter", "Theodor", "Melker", "Olle", "Viggo", "Elis", "Måns", "Ludvig",
			"Hampus", "Love", "Mio", "Mohammed", "Ali", "Edvin", "Jacob", "Samuel", "Jonathan", "Nils",
			"Ebbe", "Felix", "Hjalmar", "Kasper", "Leon", "Marcus", "Nils", "Otto", "Rasmus", "Simon",
			"Ture", "Urban", "Valdemar", "Wilhelm", "Zackarias", "Adrian", "Björn", "Carl", "David", "Emanuel",
			"Fabian", "Gabriel", "Henrik", "Ivar", "Joel", "Kalle", "Loke", "Magnus", "Niklas", "Oskar",
			"Patrik", "Robin", "Sebastian", "Teodor", "Ulrik", "Vidar", "Wilmer", "Yngve", "Zebulon",
			"Adolf", "Bengt", "Casper", "Daniel", "Einar", "Folke", "Göran", "Hans", "Ingemar", "Johan",
			"Kjell", "Lars", "Mats", "Ove", "Per", "Ragnar", "Sten", "Torsten", "Ulf",
			"Viking", "Waldemar", "Zack", "Åke", "Bertil", "Carl-Johan", "Dag", "Evert",
			"Fredrik", "Gunnar", "Håkan", "Ingvar", "Jörgen", "Kent", "Lennart", "Morgan", "Nisse", "Olof",
			"Pelle", "Rolf", "Sven", "Tor", "Ulrik", "Vilhelm", "Willy", "Zeb", "Birger",
			"Carl-Gustaf", "Dennis", "Eskil", "Finn", "Gösta", "Hampus", "Ingmar", "Jesper",
			"Krister", "Lasse", "Mårten", "Nils", "Pär", "Rune", "Stig", "Torvald", "Pål", "Alf", "Mikael",
			"Frans", "Leif", "Egon", "Sigge", "Jonas", "Orvar", "Agaton", "Ivan", "Bror", "Bosse", "Alex",
			"Otto", "Otis", "Josef", "Charles", "Östen", "Ola", "Clas", "Kurt", "Rutger", "Runar", "Felix",
			"John", "Jon", "Joakim", "Jan", "Ralph", "Måsen", "Pontus", "Börje", "Gustaf", "Fritiof", "Botvid", "Malte"
			"Anders", "Andreas", "Svante", "Tomas", "Tobias", "Sture", "Staffan", "Stellan", "Torbjörn", "Thor", "Kevin",
			"Zlatan", "Inge"
		};
	}

	constexpr auto& GetNamesByAssociation(eNationalAssociation pAssociation, auto& pNames)
	{
		const std::size_t index = static_cast<std::size_t>(pAssociation);
		return pNames[index];
	};

	template<typename T>
	constexpr auto ToVector(const auto& pArray)
	{
		return std::vector<T>(begin(pArray), end(pArray));
	}

	[[nodiscard]] inline std::vector<NamesByAssociation> GetFirstNamesByAssociation()
	{
		std::vector<NamesByAssociation> names(EnumCount<eNationalAssociation>::value);

		GetNamesByAssociation(eNationalAssociation::Belgium, names) = ToVector<std::string_view>(FirstNames::Belgium);
		GetNamesByAssociation(eNationalAssociation::England, names) = ToVector<std::string_view>(FirstNames::England);
		GetNamesByAssociation(eNationalAssociation::France, names) = ToVector<std::string_view>(FirstNames::France);
		GetNamesByAssociation(eNationalAssociation::Germany, names) = ToVector<std::string_view>(FirstNames::Germany);
		GetNamesByAssociation(eNationalAssociation::Italy, names) = ToVector<std::string_view>(FirstNames::Italy);
		GetNamesByAssociation(eNationalAssociation::Netherlands, names) = ToVector<std::string_view>(FirstNames::Netherlands);
		GetNamesByAssociation(eNationalAssociation::Nigeria, names) = ToVector<std::string_view>(FirstNames::Nigeria);
		GetNamesByAssociation(eNationalAssociation::Portugal, names) = ToVector<std::string_view>(FirstNames::Portugal);
		GetNamesByAssociation(eNationalAssociation::Spain, names) = ToVector<std::string_view>(FirstNames::Spain);
		GetNamesByAssociation(eNationalAssociation::Sweden, names) = ToVector<std::string_view>(FirstNames::Sweden);

		return names;
	};

	const std::vector<NamesByAssociation> FirstNamesByAssociation = GetFirstNamesByAssociation();


	namespace LastNames
	{


		constexpr std::array Belgium
		{
			"Peeters", "Janssens", "Maes", "Jacobs", "Mertens", "Willems", "Claes", "Goossens", "Wouters", "De Smet",
			"Dubois", "Lambert", "Dupont", "Martin", "Simon", "Dumont", "Leclercq", "Laurent", "Denis", "Lejeune",
			"Ackermans", "Daems", "De Meyer", "Gheysens", "Govaerts", "Gysbrechts", "Heylen", "Huybrechts", "Ingels", "Isebaert",
			"Janssen", "Jaspers", "Kennes", "Kusters", "Lemmens", "Martens", "Nijs", "Paulus", "Smeets", "Thijs",
			"Vermeulen", "Vandenberghe", "Vandamme", "Van den Broeck", "Van den Bossche", "Van Dyck", "Van Hoof", "Van Leeuwen", "Van Wijk", "Verhoeven",
			"Baert", "Bauwens", "Berckmans", "Blavier", "Bogaert", "Borremans", "Bosmans", "Bossuyt", "Breydel", "Breyne",
			"Broekx", "Buyse", "Cauchie", "Cerexhe", "Clerfayt", "Coclers", "Cornil", "Crugenaire", "Crusnière", "Darchis",
			"Darcis", "Darville", "De Borle", "De Caluwé", "De Crawhez", "De Lettenhove", "De Meeûs d'Argenteuil", "De Sart", "De Terwangne", "De Thozée",
			"De Wattine", "Dejeneffe", "Delbecque", "Delbrouck", "Delvaux", "Demarbaix", "Demotte", "Den Tonkelaar", "Dermont", "Despontin",
			"Dessenis", "Destatte", "Destrebecq", "Destrée", "Dewandre", "Digneffe", "Docquier", "Dodrimont", "Dolez", "Doumont",
			"Dupriez", "Durme", "Duvieusart", "Eeckhout", "Emael", "Eraerts", "Errembault", "Fagnard", "Falize", "Ficher",
			"Gadenne", "Gatz", "Gerlache", "Giet", "Golard", "Gourgue", "Grulois", "Hainaut", "Hamoir", "Harmegnies",
			"Harzé", "Harzée", "Hemroulle", "Hennaut", "Henricot", "Herbos", "Hervelle", "Herzet", "Jamoul", "Jamoulle",
			"Juseret", "Lavacherie", "Lawarrée", "Le Grelle", "Leclercq", "Lehouck", "Lelangue", "Leman", "Linard", "Linart",
			"Lizin", "Lohest", "Louesse", "Loumaye", "Louyet", "Lumay", "Lurquin", "Mahaux", "Maillen", "Maingain",
			"Malempré", "Marcourt", "Marsick", "Mawet", "Meauxsoone", "Morel-Danheel", "Nève", "Ponchau", "Ranst", "Renkin",
			"Renwa", "Rogge", "Roisin", "Rouwez", "Seny", "Soiron", "Spaak", "Spirlet", "Sweron", "Taverne",
			"Van Bilzen", "Van Branteghem", "Van Coningsloo", "Van den Kerckhove", "Van der Linden d'Hooghvorst", "Van Dhalle", "Van Dorsselaer", "Van Durme", "Van Himst", "Van Hooydonck",
			"Van Ingelgem", "Van Ingelghem", "Van Innis", "Van Landschoot", "Van Marcke", "Van Meir", "Van Meirhaeghe", "Van Melsen", "Van Nitsem", "Van Nitsen",
			"Van Overberghe", "Van Renterghem", "Van Rijmenam", "Van Robbroek", "Van Roosbroek", "Van Rymenant", "Van Santvliet", "Van Speybroek", "Van Springel", "Van Steenbergen",
			"Van Temsche", "Van Themsche", "Van Vreckom", "Van Vrekkem", "Van Waes", "Vanspeybroeck", "Vanthemsche", "Vanwesenbeeck", "Verhelst", "Vliebergh",
			"Waha", "Wallon", "Warin", "Warnier", "Warocqué", "Wathelet", "Watillion", "Watillon", "Watrin", "Wattergniaux",
			"Watthy", "Wauthier", "Wauthion", "Wautrin", "Wiart", "Wigny", "Wilmet", "Winquin", "Woelmont", "Baudouin",
			"Benoît", "Bernard", "Bertrand", "Blanchet", "Blondel", "Boucher", "Boulanger", "Bourgeois", "Brunet", "Carpentier",
			"Charpentier", "Chevalier", "Clément", "Collet", "Collette", "Cousin", "Delacroix", "Delage", "Delannoy", "Delattre",
			"Delcroix", "Delfosse", "Delhomme", "Delorme", "Delvigne", "Delville", "Deschamps", "Desmet", "Devaux", "Devos",
			"Duchêne", "Duchemin", "Dufour", "Dujardin", "Dumoulin", "Dupuis", "Durand", "Fabre", "Faucher", "Fauconnier",
			"Fournier", "Gagnon", "Gauthier", "Gérard", "Gillet", "Giraud", "Gosselin", "Grégoire", "Guérin", "Guillaume",
			"Hamelin", "Hébert", "Hennequin", "Hernandez", "Hervé", "Hubert", "Joubert", "Lacroix", "Lafleur", "Lamarche",
			"Lapointe", "Lavigne", "Lemoine", "Lévesque", "Marchand", "Maréchal", "Mercier", "Moreau", "Morin", "Moulin",
		};

		constexpr std::array England
		{
			"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", "Davis", "Rodriguez", "Martinez",
			"Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson", "Thomas", "Taylor", "Moore", "Martin", "Lee",
			"Perez", "Thompson", "White", "Harris", "Sanchez", "Clark", "Ramirez", "Lewis", "Robinson", "Walker",
			"Young", "Allen", "King", "Wright", "Scott", "Green", "Baker", "Adams", "Nelson", "Hill",
			"Campbell", "Mitchell", "Roberts", "Carter", "Phillips", "Evans", "Turner", "Torres", "Parker", "Collins",
			"Edwards", "Stewart", "Flores", "Morris", "Nguyen", "Murphy", "Rivera", "Cook", "Rogers", "Morgan",
			"Peterson", "Cooper", "Reed", "Bailey", "Bell", "Gomez", "Kelly", "Howard", "Ward", "Brooks",
			"Diaz", "Ramirez", "Watson", "Sanders", "Price", "Bennett", "Wood", "Barnes", "Ross", "Henderson",
			"Coleman", "Jenkins", "Perry", "Powell", "Long", "Patterson", "Hughes", "Flores", "Washington", "Butler",
			"Simmons", "Foster", "Gonzales", "Bryant", "Alexander", "Russell", "Griffin", "Diaz", "Hayes", "Myers",
			"Ford", "Hamilton", "Graham", "Sullivan", "Wallace", "Woods", "Cole", "West", "Jordan", "Owens",
			"Reynolds", "Fisher", "Ellis", "Andrews", "Marshall", "Murray", "Freeman", "Harrison", "McDonald", "Cruz",
			"Edwards", "James", "Reid", "Stevens", "Park", "Morrison", "Munoz", "Knight", "Walters", "Ruiz",
			"Hunter", "Lane", "Riley", "Armstrong", "Carroll", "Weaver", "Greene", "Lawrence", "Elliott", "Chavez",
			"Sims", "Austin", "Peters", "Kelley", "Franklin", "Lawson", "Fields", "Gutierrez", "Ryan", "Schmidt",
			"Carr", "Vasquez", "Castillo", "Wheeler", "Chapman", "Oliver", "Montgomery", "Richards", "Williamson", "Johnston",
			"Banks", "Meyer", "Bishop", "McCoy", "Howell", "Alvarez", "Morrison", "Hansen", "Fernandez", "Garza",
			"Harvey", "Little", "Burton", "Stanley", "Nguyen", "George", "Jacobs", "Reid", "Kim", "Fuller",
			"Lynch", "Dean", "Gilbert", "Garrett", "Romero", "Welch", "Larson", "Frazier", "Burke", "Hanson",
			"Day", "Mendoza", "Moreno", "Bowman", "Medina", "Fowler", "Brewer", "Hoffman", "Carlson", "Silva",
			"Pearson", "Holland", "Douglas", "Fleming", "Jensen", "Vargas", "Byrd", "Davidson", "Hopkins", "May",
			"Terry", "Herrera", "Wade", "Soto", "Walsh", "Curtis", "Neal", "Caldwell", "Lowe", "Jennings",
			"Barnett", "Graves", "Jimenez", "Horton", "Shelton", "Barrett", "O'Brien", "Gregory", "Mckinney", "Lucas",
			"Miles", "Craig", "Rodriquez", "Chambers", "Holt", "Lambert", "Fletcher", "Watts", "Bates", "Hale",
			"Rhodes", "Pena", "Beck", "Newman", "Haynes", "Mcdaniel", "Mendez", "Bush", "Vaughn", "Parks",
			"Dawson", "Santiago", "Norris", "Hardy", "Love", "Steele", "Curry", "Powers", "Schultz", "Barker",
			"Guzman", "Page", "Munoz", "Ball", "Keller", "Chandler", "Weber", "Leonard", "Walton", "Lyons",
			"Ramsey", "Wolfe", "Schneider", "Mullins", "Benson", "Sharp", "Bowen", "Daniel", "Barber", "Cummings",
			"Hines", "Baldwin", "Griffith", "Valdez", "Hubbard", "Salazar", "Reeves", "Warner", "Stevenson", "Burgess",
			"Santana", "Tate", "Cross", "Garner", "Mann", "Mack", "Moss", "Thornton", "Dennis", "Mcgee",
		};

		constexpr std::array France
		{
			"Abadie", "Abreo", "Alarie", "Archambeau", "Auch", "Badeaux", "Basset", "Baudelaire", "Beaufort", "Bois",
			"Cadieux", "Carbonneau", "Castillon", "Chapelle", "Chapdelaine", "Chevrolet", "DeRose", "Desjardins", "De Villiers",
			"Du Bellay", "Edouard", "Farrow", "Forester", "Fortin", "Fraise", "Frossard", "Gagneux", "Gauthier", "Gay",
			"Gouin", "Heroux", "Houde", "Hubert", "Jacques", "Janvier", "Joubert", "Jourdain", "Kaplan", "Lacroix",
			"Laflamme", "Lane", "Langlais", "La Rue", "Laurel", "Lavoie", "Le Beau", "Leroy", "Marchand", "Matthieu",
			"Meunier", "Montagne", "Noyer", "Ozanne", "Page", "Palomer", "Paquet", "Pascal", "Périgord", "Picard",
			"Plantier", "Plourde", "Rayne", "Renaud", "Reason", "Remy", "Richard", "Richelieu", "Riviere", "Robida",
			"Rodier", "Sartre", "Sauveterre", "Segal", "Serres", "Simon", "Soulier", "Tasse", "Tavernier", "Thibaut",
			"Travers", "Trottier", "Vachon", "Vaillancourt", "Vaillant", "Valade", "Vannier", "Varne", "Varon", "Vautour",
			"Vaux", "Vernier", "Vigneau", "Vinet", "Visage", "Voclian", "Voland", "Wack", "Webster", "Yolande", "Yotte",
			"Zabelle", "Allard", "Anouilh", "Aubert", "Auclair", "Bain", "Barbier", "Beaumont", "Boucher", "Cartier",
			"Cellier", "Dubois", "Dupont", "Durand", "Escoffier", "Faucher", "Fontaine", "Garcon", "Garnier", "Guillaume",
			"Lambert", "Lavigne", "Lefevre", "Moreau", "Martin", "Monet", "Moulin", "Roche", "Toussaint", "Tremblay",
			"Auger", "Barbet", "Bassett", "Beausoleil", "Belshaw", "Bernard", "Blanc", "Boisseau", "Bonhomme", "Breguet",
			"Carpentier", "Chappuis", "Chevalier", "Collin", "Couture", "Deloney", "Fay", "Gaudin", "Grange", "Lagarde",
			"Leclerc", "Lemaitre", "Loup", "Pelissier", "Pomeroy", "Proulx", "Rousseau", "Serrurier", "Verne", "Villeneuve",
			"Auguste", "Bardin", "Bastien", "Bechard", "Bernier", "Blanchard", "Boivin", "Bonne", "Bruneau", "Carre",
			"Charbonnier", "Chopin", "Comtois", "Crozier", "Droit", "Forestier", "Geiger", "Lagrange", "Lamar", "Leger",
			"Lenoir", "Marchal", "Plessis", "Porcher", "Renouard", "Rutter", "Tardy", "Verville", "Violette", "Aveline",
			"Barreau", "Bazin", "Bedeau", "Berengar", "Bethune", "Blanchet", "Bonfils", "Borde", "Bullion", "Carrel",
			"Chastain", "Clair", "Corbin", "Dupuis", "Fortier", "Fournier", "Gerard", "Landry", "Langlois", "Lejeune",
			"Leroux", "Martel", "Poirier", "Poulin", "Ricard", "Sale", "Veilleux", "Vigne", "Voclain",
		};

		constexpr std::array Germany
		{
			"Müller", "Schmidt", "Schneider", "Fischer", "Weber", "Meyer", "Wagner", "Becker", "Schulz", "Hoffmann",
			"Schäfer", "Koch", "Bauer", "Richter", "Klein", "Wolf", "Schröder", "Neumann", "Schwarz", "Zimmermann",
			"Braun", "Krüger", "Hofmann", "Hartmann", "Lange", "Schmitt", "Werner", "Schmitz", "Krause", "Meier",
			"Lehmann", "Schmid", "Schulze", "Maier", "Köhler", "Herrmann", "König", "Walter", "Mayer", "Huber",
			"Kaiser", "Fuchs", "Peters", "Lang", "Scholz", "Möller", "Weiß", "Jung", "Hahn", "Schubert",
			"Vogel", "Friedrich", "Keller", "Günther", "Frank", "Berger", "Winkler", "Roth", "Beck", "Lorenz",
			"Baumann", "Franke", "Albrecht", "Schuster", "Simon", "Ludwig", "Böhm", "Winter", "Kraus", "Martin",
			"Schumacher", "Krämer", "Vogt", "Stein", "Jäger", "Otto", "Sommer", "Groß", "Seidel", "Heinrich",
			"Brandt", "Haas", "Schreiber", "Graf", "Schulte", "Dietrich", "Ziegler", "Kuhn", "Kühn", "Pohl",
			"Engel", "Horn", "Busch", "Bergmann", "Thomas", "Voigt", "Sauer", "Arnold", "Wolff", "Bender",
			"Pfeiffer", "Altergott", "Bierhals", "Durchdenwald", "Handschuh", "Nachtnebel", "Trinkenschuh", "Abbas",
			"Adler", "Bach", "Beethoven", "Einstein", "Freud", "Goethe", "Kant", "Schneider", "Schumacher", "Wagner",
			"Geiger", "Graf", "Hartmann", "Klein", "König", "Lehmann", "Müller", "Meier", "Neumann", "Richter",
			"Schmidt", "Schulz", "Baumann", "Braun", "Dietrich", "Engel", "Fischer", "Frank", "Friedrich", "Günther",
			"Haas", "Hartmann", "Hoffmann", "Horn", "Jäger", "Kaiser", "Keller", "Klein", "Koch", "König",
			"Krause", "Krüger", "Kuhn", "Kühn", "Lang", "Lange", "Lehmann", "Lorenz", "Maier", "Martin",
			"Mayer", "Meier", "Meyer", "Möller", "Müller", "Neumann", "Otto", "Peters", "Pfeiffer", "Pohl",
			"Richter", "Roth", "Sauer", "Schäfer", "Schmidt", "Schmitt", "Schmitz", "Schneider", "Scholz",
			"Schröder", "Schulte", "Schulz", "Schumacher", "Schuster", "Schwarz", "Seidel", "Simon", "Sommer",
			"Stein", "Thomas", "Vogel", "Vogt", "Voigt", "Walter", "Weber", "Weiß", "Werner", "Winkler",
			"Winter", "Wolf", "Wolff", "Ziegler", "Zimmermann", "Albrecht", "Arnold", "Baumann", "Beck",
			"Berger", "Bergmann", "Brandt", "Braun", "Böhm", "Busch", "Dietrich", "Engel", "Fischer", "Frank",
			"Franke", "Friedrich", "Fuchs", "Gärtner", "Geiger", "Graf", "Groß", "Haas", "Hahn", "Hartmann",
			"Heinrich", "Hoffmann", "Horn", "Huber", "Jäger", "Kaiser", "Keller", "Klein", "Koch", "König",
			"Krause", "Kraus", "Krämer", "Krüger", "Kuhn", "Kühn", "Lang", "Lange", "Lehmann", "Lorenz",
			"Ludwig", "Maier", "Martin", "Mayer", "Meier", "Meyer", "Möller", "Müller", "Neumann", "Otto",
			"Pfeiffer", "Pohl", "Richter", "Roth", "Sauer", "Schäfer", "Schmidt", "Schmitt", "Schmitz", "Schneider",
			"Scholz", "Schröder", "Schulte", "Schulz", "Schumacher", "Schuster", "Schwarz", "Seidel", "Simon",
			"Sommer", "Stein", "Thomas", "Vogel", "Vogt", "Voigt", "Walter", "Weber", "Weiß", "Werner",
			"Winkler", "Winter", "Wolf", "Wolff", "Ziegler", "Zimmermann"
		};

		constexpr std::array Netherlands
		{
			"de Jong", "Jansen", "de Vries", "Bakker", "Janssen", "van Dijk", "Visser", "Smit", "de Boer", "de Groot",
			"Mulder", "Meijer", "Bos", "Vos", "Peters", "van der Berg", "Hendriks", "Dekker", "van Leeuwen", "Brouwer",
			"de Wit", "Dijkstra", "Smits", "de Graaf", "van den Heuvel", "van der Meer", "Kok", "Jacobs", "de Haan", "van Beek",
			"Willems", "Hoekstra", "Maas", "Schouten", "Veenstra", "van Vliet", "van Dam", "van der Linden", "Prins", "Gerritsen",
			"van den Bosch", "van der Wal", "Blom", "Kuipers", "Vermeulen", "van der Veen", "Postma", "Hermans", "Martens", "van Loon",
			"de Bruin", "Scholten", "Boer", "Koster", "de Ruiter", "van Wijk", "van der Heijden", "van der Ven", "Schipper", "Sanders",
			"van der Heijden", "de Leeuw", "van den Broek", "de Vos", "van der Pol", "Hofman", "de Lange", "van der Meulen", "van der Horst", "de Koning",
			"van der Laan", "de Ruiter", "van der Plas", "van der Velden", "de Jonge", "van der Zanden", "de Klerk", "van der Zee", "van der Werf", "van der Linde",
			"de Roos", "van der Meulen", "van der Velde", "van der Weide", "van der Woude", "van der Zwan", "van der Zwart",
		};

		constexpr std::array Italy
		{
			"Rossi", "Russo", "Ferrari", "Esposito", "Bianchi", "Romano", "Colombo", "Bruno", "Ricci", "Marino",
			"Greco", "Gallo", "Conti", "De Luca", "Costa", "Mancini", "Giordano", "Rizzo", "Lombardi", "Moretti",
			"Barbieri", "Fontana", "Santoro", "Mariani", "Rinaldi", "Caruso", "Ferrara", "Galli", "Martini", "Leone",
			"Longo", "Gatti", "Serra", "Coppola", "De Santis", "D'Angelo", "Fiore", "Monti", "Palumbo", "Battaglia",
			"Valentini", "Pellegrini", "Ruggiero", "Sorrentino", "Orlando", "Pagano", "Caputo", "Benedetti", "Bellini", "Rossetti",
			"Piras", "Riva", "Montanari", "Rinaldo", "Sanna", "Grassi", "Testa", "De Rosa", "Farina", "Neri",
			"Pellegrino", "Basile", "Giuliani", "Martino", "Morelli", "De Angelis", "Silvestri", "Lombardo", "Gentile", "Barone",
			"Fabbri", "Grasso", "Fiorelli", "Ferraro", "Guerra", "Firetti", "de Rossi", "Limone"
		};

		constexpr std::array Nigeria
		{
			"Aboderin", "Abogunrin", "Abubakar", "Adebanjo", "Adegbite", "Adegoke", "Adejumobi", "Adelakun", "Adeniran", "Adewale",
			"Adeyemi", "Afolabi", "Ajayi", "Akinbode", "Akinjide", "Akinsanya", "Alabi", "Anyanwu", "Awolowo", "Azikiwe",
			"Babalola", "Balogun", "Bamgbose", "Bankole", "Chinwuba", "Chukwuma", "Daramola", "Dike", "Ekwueme", "Eze",
			"Fagbenro", "Fashola", "Fatunmbi", "Gbadebo", "Ibukun", "Ijagbemi", "Ilori", "Isiba", "Itua", "Jegede",
			"Jimoh", "Kasali", "Kolapo", "Majekodunmi", "Makinde", "Makanjuola", "Mayowa", "Mogaji", "Mohammed", "Morenikeji",
			"Nwankwo", "Nwachukwu", "Nwaeze", "Obafemi", "Odunayo", "Ogundare", "Ogunleye", "Ojo", "Okafor", "Okeke",
			"Olaniran", "Olayemi", "Olayinka", "Olufunmilayo", "Olumorin", "Olusanya", "Omoniyi", "Omotayo", "Onifade", "Onikoyi",
			"Orimolade", "Oyelowo", "Oluwaseun", "Oluwaseyi", "Oluwadamilola", "Olumide", "Olamilekan", "Olajide", "Oladipo",
			"Olumuyiwa", "Olufemi", "Olajuwon", "Olakunle", "Olarenwaju", "Olamide", "Oluwaseun", "Olufunmilayo", "Ogunfowokan",
			"Oludare", "Adebisi", "Adegbola", "Adeola", "Ademola", "Adesanya", "Afolayan", "Agboola", "Akinfenwa", "Akinyele",
			"Akpan", "Alao", "Alhaji", "Aluko", "Amos", "Anyanwu", "Ayanwoye", "Babatunde", "Babalola", "Babajide",
			"Bamidele", "Banjo", "Baruwa", "Bayo", "Bolanle", "Bolu", "Bose", "Chijioke", "Chuka", "Chukwudi", "Chukwuka",
			"Chinonso", "Chizor", "Chukwuma", "Dada", "Daramola", "Darasimi", "Dele", "Durojaiye", "Durojaye", "Durotoye",
			"Duro", "Ekundayo", "Ebele", "Echendu", "Efe", "Egbe", "Egbuna", "Ekene", "Ekerete", "Emeka", "Eze", "Fatai",
			"Fashola", "Fashokun", "Femi", "Folake", "Folorunso", "Folarin", "Ganiyu", "Gbadebo", "Gbenga", "Gbolahan",
			"Goke", "Idris", "Ifeanyi", "Ifeanyichukwu", "Ikechukwu", "Ikenna", "Iliya", "Ikemefuna", "Iroegbu", "Ismail",
			"Jibola", "Jide", "Jibril", "Jude", "Kazeem", "Kehinde", "Kolawole", "Ladipo", "Lekan", "Lola", "Micheal",
			"Mogaji", "Mohammed", "Moses", "Murtala", "Nduka", "Niyi", "Nnamdi", "Nwachukwu", "Olafemi", "Olalekan", "Olamide",
			"Olanrewaju", "Olaseni", "Olu", "Oluwadamilola", "Olumide", "Olumuyiwa", "Oluwaseyi", "Olufemi", "Olutayo", "Oluwatobi",
			"Oluwasegun", "Oluwaseun", "Oluwatamilore", "Oluwatimilehin", "Oluwafemi", "Onyekachi", "Onyeka", "Oreoluwa", "Opeyemi",
			"Orobosa", "Orji", "Osawe", "Osagie", "Osemwinde", "Raji", "Rilwan", "Rasaq", "Rauf", "Saka", "Sulaimon", "Samuel",
			"Segun", "Seyi", "Simeon", "Taiwo", "Tariq", "Tarique", "Temidayo", "Temiloluwa", "Titi", "Titiola", "Tolu",
			"Tobiloba", "Tola", "Toluwani", "Tosin", "Toye", "Tundun", "Tunde", "Tunji", "Toyin", "Uche", "Uchechukwu",
			"Ugochukwu", "Umar", "Victor", "Wasiu", "Yemi", "Yusuf", "Zainab", "Zubair", "Zubairu", "Ajibola", "Akinbode",
			"Akinfe", "Akinfenwa", "Akingbade", "Akinsanya", "Akinyemi", "Alade", "Alhaji", "Amodu", "Amos", "Anjorin",
			"Anibaba", "Ayinde", "Azikiwe", "Bamidele", "Balogun", "Bamgbose", "Bamimore", "Bello", "Bello", "Chuka", "Chukwudi",
			"Chukwuemeka", "Chukwuma", "Dada", "Daramola", "Durojaiye", "Ekeleme", "Eze", "Ezekiel", "Fashola", "Fatunmbi",
			"Fayemi", "Fayose", "Gbadamosi", "Ganiyu", "Gbolahan", "Gbenga", "Goke", "Ifeoluwa", "Ifeanyichukwu", "Idris",
			"Ikechukwu", "Jibola", "Jide", "Kazeem", "Kehinde", "Kolapo", "Lekan", "Ladipo", "Micheal", "Murtala", "Moses",
			"Modupe"
		};

		constexpr std::array Portugal
		{
			"Silva", "Santos", "Pereira", "Oliveira", "Costa", "Rodrigues", "Martins", "Ferreira", "Almeida", "Ribeiro",
			"Gomes", "Lopes", "Fernandes", "Gonçalves", "Mendes", "Carvalho", "Soares", "Barbosa", "Cardoso", "Pinto",
			"Alves", "Monteiro", "Moreira", "Machado", "Rocha", "Nunes", "Marques", "Dias", "Castro", "Coelho",
			"Araújo", "Sousa", "Cruz", "Matos", "Magalhães", "Jesus", "Reis", "Correia", "Maia", "Amorim",
			"Baptista", "Guedes", "Pinheiro", "Vieira", "Borges", "Cunha", "Azevedo", "Ramos", "Simões", "Pires",
			"Tavares", "Leite", "Teixeira", "Fonseca", "Mendonça", "Miranda", "Abreu", "Freitas", "Andrade", "Sá",
			"Nogueira", "Valente", "Vaz", "Mota", "Lima", "Dantas", "Espinho", "Falcão", "Figueiredo", "Gama",
			"Guimarães", "Henriques", "Lourenço", "Marinho", "Moura", "Neto", "Pacheco", "Queirós", "Rosa", "Sampaio",
			"Sequeira", "Serra", "Torres", "Vargas", "Vidal", "Xavier", "Aguiar", "Albuquerque", "Almada", "Amaral",
			"Antunes", "Aragão", "Avelar", "Basto", "Brito", "Cabral", "Caldas", "Camacho", "Campos", "Câmara",
			"Canedo", "Capela", "Carneiro", "Carreira", "Casimiro", "Castanheira", "Catarino", "Cavalcanti", "Chaves", "Clemente",
			"Conceição", "Cordeiro", "Corte-Real", "Coutinho", "Couto", "Crespo", "Custódio", "Dâmaso", "Domingues", "Duarte",
			"Esteves", "Falcão", "Faria", "Farinha", "Feijó", "Félix", "Ferraz", "Figueira", "Fontes", "Franco",
			"Fraga", "Freire", "Furtado", "Galvão", "Garcia", "Gaspar", "Gato", "Gouveia", "Graça", "Guerreiro",
			"Henriques", "Herculano", "Horta", "Infante", "Jacinto", "Januário", "Jardim", "Jesus", "Joaquim", "Jorge",
			"Júlio", "Justino", "Lacerda", "Lagos", "Lameira", "Lança", "Leal", "Lemos", "Loureiro", "Lourenço",
			"Luz", "Machado", "Madeira", "Magalhães",  "Malheiro", "Matias", "Macedo", "Melo", "Mendonça", "Mesquita", "Monteiro", "Morgado", "Mota", "Moura",
			"Mourão", "Nascimento", "Neves", "Nogueira", "Noronha", "Novaes", "Novais", "Oliveira", "Ortigão",
			"Paiva", "Paixão", "Palma", "Parente", "Pascoal", "Patrício", "Pedreira", "Pereira", "Pimenta", "Pimentel",
			"Pinho", "Pires", "Ponte", "Portela", "Porto", "Pousão", "Quadros", "Queirós", "Quintela", "Ramalho",
			"Ramos", "Rebelo", "Rego", "Resende", "Ribeiro", "Rocha", "Rodrigues", "Roque", "Rosa",
			"Sá", "Salgado", "Salgueiro", "Sampaio", "Santana", "Santos", "Saraiva", "Sardinha", "Seixas", "Serra",
			"Silva", "Simões", "Soares", "Sousa", "Soveral", "Tavares", "Teixeira", "Teles", "Torres", "Trindade",
			"Valadares", "Valente", "Valença", "Varela", "Vasconcelos", "Vaz", "Veiga", "Veloso", "Ventura", "Viana",
			"Vidal", "Vieira", "Vilaça", "Vilas", "Vilela", "Xavier", "Zambujo", "Abrantes", "Abreu", "Afonso",
			"Agostinho", "Aires", "Alcântara", "Alcoforado", "Alegria", "Amado",
			"Anjos", "Antas", "Antunes", "Avelar", "Azevedo", "Baptista", "Barata", "Barbosa",
			"Barreto", "Barros", "Bastos", "Batista", "Belo", "Bettencourt", "Botelho", "Braga", "Branco",
			"Brandão", "Caetano", "Caldeira", "Camacho", "Campos", "Candeias", "Caneira"
		};

		constexpr std::array Spain
		{
			"García", "Fernández", "González", "Rodríguez", "López", "Martínez", "Sánchez", "Pérez", "Gómez", "Martín",
			"Jiménez", "Ruiz", "Hernández", "Díaz", "Álvarez", "Moreno", "Muñoz", "Navarro", "Gutiérrez", "Romero",
			"Torres", "Ramírez", "Flores", "Ramirez", "Vázquez", "Castro", "Ortiz", "Rubio", "Serrano", "Molina",
			"Morales", "Suárez", "Cruz", "Reyes", "Mendoza", "Aguilar", "Guzmán", "Fernandez", "Domínguez", "Rivera",
			"Iglesias", "Luna", "Medina", "Soto", "Cortez", "Delgado", "Castillo", "Ortega", "Herrera", "Velásquez",
			"Vargas", "Cabrera", "Campos", "Ávila", "Pacheco", "Sandoval", "Arias", "Vega", "Núñez", "Aguirre",
			"Orozco", "Salazar", "Estrada", "Bustos", "Carrillo", "Padilla", "Rojas", "Silva", "Guerrero", "Contreras",
			"León", "Calderón", "Rosales", "Maldonado", "Peña", "Ríos", "Cárdenas", "Alvarado", "Castañeda", "Tapia",
			"Lara", "Fuentes", "Quintana", "Miranda", "Valenzuela", "Zúñiga", "Peralta", "Pineda", "Rangel", "Aguilera",
			"Ochoa", "Solís", "Rivas", "Mora", "Cordero", "Villanueva", "Marín", "Crespo", "Blanco", "Bravo",
			"Márquez", "Olivera", "Montoya", "Zapata", "Cuevas", "Ojeda", "Rosario", "Cisneros", "Valverde", "Acevedo",
			"Roldán", "Paz", "Yáñez", "Acosta", "Molano", "Barrera", "Castaño", "Villalobos", "Galván", "Salinas",
			"Escobar", "Valencia", "Baez", "Caballero", "Ayala", "Villalba", "Nava", "Arroyo", "Santana", "Covarrubias",
			"Vidal", "Gallegos", "Casillas", "Varela", "Lozano", "Parra", "Correa", "Rojas", "Mercado", "Cordova",
			"Mata", "Valdés", "Paniagua", "Santos", "Castaño", "Galindo", "Montes", "Ruíz", "Villarreal", "Rincón",
			"Carrasco", "Carranza", "Villalpando", "Carrera", "Carrasquillo", "Carrillo", "Carrion"
		};

		constexpr std::array Sweden
		{
			"Johansson", "Andersson", "Karlsson", "Nilsson", "Eriksson", "Larsson", "Olsson", "Persson", "Svensson", "Gustafsson",
			"Pettersson", "Jonsson", "Jansson", "Hansson", "Bengtsson", "Carlsson", "Jönsson", "Petersson", "Magnusson", "Gustavsson",
			"Olofsson", "Lindberg", "Jakobsson", "Lindström", "Lindqvist", "Lindgren", "Bergström", "Sjöberg", "Sandberg", "Lundberg",
			"Berg", "Holm", "Lundgren", "Björk", "Bergman", "Lind", "Ekström", "Engström", "Berglund", "Bergqvist",
			"Lundqvist", "Lindholm", "Ström", "Sundberg", "Eklund", "Forsberg", "Nyström", "Lundström", "Wallin", "Hellström",
			"Dahl", "Hedlund", "Sjögren", "Åberg", "Nordström", "Löfgren", "Sandström", "Håkansson", "Blom", "Sundström",
			"Lundin", "Boström", "Blomqvist", "Fransson", "Nordin", "Mårtensson", "Berggren", "Lindén", "Sjöström", "Bergström",
			"Lindblad", "Bergquist", "Lindahl", "Sundqvist", "Lindkvist", "Bergkvist", "Lindskog", "Lindholm",
		};

	}

	[[nodiscard]] inline std::vector<NamesByAssociation> GetLastNamesByAssociation()
	{
		std::vector<NamesByAssociation> names(EnumCount<eNationalAssociation>::value);

		GetNamesByAssociation(eNationalAssociation::Belgium, names) = ToVector<std::string_view>(LastNames::Belgium);
		GetNamesByAssociation(eNationalAssociation::England, names) = ToVector<std::string_view>(LastNames::England);
		GetNamesByAssociation(eNationalAssociation::France, names) = ToVector<std::string_view>(LastNames::France);
		GetNamesByAssociation(eNationalAssociation::Germany, names) = ToVector<std::string_view>(LastNames::Germany);
		GetNamesByAssociation(eNationalAssociation::Italy, names) = ToVector<std::string_view>(LastNames::Italy);
		GetNamesByAssociation(eNationalAssociation::Netherlands, names) = ToVector<std::string_view>(LastNames::Netherlands);
		GetNamesByAssociation(eNationalAssociation::Nigeria, names) = ToVector<std::string_view>(LastNames::Nigeria);
		GetNamesByAssociation(eNationalAssociation::Portugal, names) = ToVector<std::string_view>(LastNames::Portugal);
		GetNamesByAssociation(eNationalAssociation::Spain, names) = ToVector<std::string_view>(LastNames::Spain);
		GetNamesByAssociation(eNationalAssociation::Sweden, names) = ToVector<std::string_view>(LastNames::Sweden);

		return names;
	};

	const std::vector<NamesByAssociation> LastNamesByAssociation = GetLastNamesByAssociation();

}