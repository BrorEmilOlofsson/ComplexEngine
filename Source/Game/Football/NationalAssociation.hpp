#pragma once

#if 1
enum class eNationalAssociation
{
	Belgium,
	England,
	France,
	Germany,
	Italy,
	Netherlands,
	Nigeria,
	Portugal,
	Spain,
	Sweden,
	Count
};
#else
enum class eNationalAssociation
{
	Afghanistan,
	Albania,
	Algeria,
	Andorra,
	Angola,
	AntiguaAndBarbuda,
	Argentina,
	Armenia,
	Australia,
	Austria,
	Azerbaijan,
	Bahamas,
	Bahrain,
	Bangladesh,
	Barbados,
	Belarus,
	Belgium,
	Belize,
	Benin,
	Bermuda,
	Bhutan,
	Bolivia,
	BosniaAndHerzegovina,
	Botswana,
	Brazil,
	Brunei,
	Bulgaria,
	BurkinaFaso,
	Burundi,
	CaboVerde,
	Cambodia,
	Cameroon,
	Canada,
	CentralAfricanRepublic,
	Chad,
	Chile,
	ChinaPR,
	Colombia,
	Comoros,
	Congo,
	CongoDR,
	CookIslands,
	CostaRica,
	CoteDIvoire,
	Croatia,
	Cuba,
	Curacao,
	Cyprus,
	CzechRepublic,
	Denmark,
	Djibouti,
	Dominica,
	DominicanRepublic,
	Ecuador,
	Egypt,
	ElSalvador,
	England,
	EquatorialGuinea,
	Eritrea,
	Estonia,
	Eswatini,
	Ethiopia,
	FaroeIslands,
	Fiji,
	Finland,
	France,
	Gabon,
	Gambia,
	Georgia,
	Germany,
	Ghana,
	Gibraltar,
	Greece,
	Grenada,
	Guam,
	Guatemala,
	Guinea,
	GuineaBissau,
	Guyana,
	Haiti,
	Honduras,
	HongKong,
	Hungary,
	Iceland,
	India,
	Indonesia,
	Iran,
	Iraq,
	Ireland,
	Israel,
	Italy,
	Jamaica,
	Japan,
	Jordan,
	Kazakhstan,
	Kenya,
	Kiribati,
	Kosovo,
	Kuwait,
	Kyrgyzstan,
	Laos,
	Latvia,
	Lebanon,
	Lesotho,
	Liberia,
	Libya,
	Liechtenstein,
	Lithuania,
	Luxembourg,
	Macau,
	Madagascar,
	Malawi,
	Malaysia,
	Maldives,
	Mali,
	Malta,
	MarshallIslands,
	Mauritania,
	Mauritius,
	Mexico,
	Micronesia,
	Moldova,
	Monaco,
	Mongolia,
	Montenegro,
	Montserrat,
	Morocco,
	Mozambique,
	Myanmar,
	Namibia,
	Nauru,
	Nepal,
	Netherlands,
	NewCaledonia,
	NewZealand,
	Nicaragua,
	Niger,
	Nigeria,
	NorthKorea,
	NorthMacedonia,
	NorthernIreland,
	Norway,
	Oman,
	Pakistan,
	Palestine,
	Panama,
	PapuaNewGuinea,
	Paraguay,
	Peru,
	Philippines,
	Poland,
	Portugal,
	PuertoRico,
	Qatar,
	Romania,
	Russia,
	Rwanda,
	SaintKittsAndNevis,
	SaintLucia,
	SaintVincentAndTheGrenadines,
	Samoa,
	SanMarino,
	SaoTomeAndPrincipe,
	SaudiArabia,
	Scotland,
	Senegal,
	Serbia,
	Seychelles,
	SierraLeone,
	Singapore,
	Slovakia,
	Slovenia,
	SolomonIslands,
	Somalia,
	SouthAfrica,
	SouthKorea,
	SouthSudan,
	Spain,
	SriLanka,
	Sudan,
	Suriname,
	Sweden,
	Switzerland,
	Syria,
	Taiwan,
	Tajikistan,
	Tanzania,
	Thailand,
	TimorLeste,
	Togo,
	Tonga,
	TrinidadAndTobago,
	Tunisia,
	Turkey,
	Turkmenistan,
	TurksAndCaicosIslands,
	Tuvalu,
	Uganda,
	Ukraine,
	UnitedArabEmirates,
	UnitedStates,
	Uruguay,
	USVirginIslands,
	Uzbekistan,
	Vanuatu,
	VaticanCity,
	Venezuela,
	Vietnam,
	Wales,
	Yemen,
	Zambia,
	Zimbabwe
};
#endif

template<typename>
struct EnumCount;

template<>
struct EnumCount<eNationalAssociation> : std::integral_constant<std::size_t, static_cast<std::size_t>(eNationalAssociation::Count)> {};

constexpr std::string EnumToString(eNationalAssociation pAssociation)
{
	switch (pAssociation)
	{
	case eNationalAssociation::Belgium: return "Belgium";
	case eNationalAssociation::Italy: return "Italy";
	case eNationalAssociation::England: return "England";
	case eNationalAssociation::France: return "France";
	case eNationalAssociation::Germany: return "Germany";
	case eNationalAssociation::Netherlands: return "Netherlands";
	case eNationalAssociation::Nigeria: return "Nigeria";
	case eNationalAssociation::Portugal: return "Portugal";
	case eNationalAssociation::Spain: return "Spain";
	case eNationalAssociation::Sweden: return "Sweden";
#if 0
	case eNationalAssociation::Afghanistan: return "Afghanistan";
	case eNationalAssociation::Albania: return "Albania";
	case eNationalAssociation::Algeria: return "Algeria";
	case eNationalAssociation::Andorra: return "Andorra";
	case eNationalAssociation::Angola: return "Angola";
	case eNationalAssociation::AntiguaAndBarbuda: return "Antigua and Barbuda";
	case eNationalAssociation::Argentina: return "Argentina";
	case eNationalAssociation::Armenia: return "Armenia";
	case eNationalAssociation::Australia: return "Australia";
	case eNationalAssociation::Austria: return "Austria";
	case eNationalAssociation::Azerbaijan: return "Azerbaijan";
	case eNationalAssociation::Bahamas: return "Bahamas";
	case eNationalAssociation::Bahrain: return "Bahrain";
	case eNationalAssociation::Bangladesh: return "Bangladesh";
	case eNationalAssociation::Barbados: return "Barbados";
	case eNationalAssociation::Belarus: return "Belarus";
	case eNationalAssociation::Belgium: return "Belgium";
	case eNationalAssociation::Belize: return "Belize";
	case eNationalAssociation::Benin: return "Benin";
	case eNationalAssociation::Bermuda: return "Bermuda";
	case eNationalAssociation::Bhutan: return "Bhutan";
	case eNationalAssociation::Bolivia: return "Bolivia";
	case eNationalAssociation::BosniaAndHerzegovina: return "Bosnia and Herzegovina";
	case eNationalAssociation::Botswana: return "Botswana";
	case eNationalAssociation::Brazil: return "Brazil";
	case eNationalAssociation::Brunei: return "Brunei";
	case eNationalAssociation::Bulgaria: return "Bulgaria";
	case eNationalAssociation::BurkinaFaso: return "Burkina Faso";
	case eNationalAssociation::Burundi: return "Burundi";
	case eNationalAssociation::CaboVerde: return "Cabo Verde";
	case eNationalAssociation::Cambodia: return "Cambodia";
	case eNationalAssociation::Cameroon: return "Cameroon";
	case eNationalAssociation::Canada: return "Canada";
	case eNationalAssociation::CentralAfricanRepublic: return "Central African Republic";
	case eNationalAssociation::Chad: return "Chad";
	case eNationalAssociation::Chile: return "Chile";
	case eNationalAssociation::ChinaPR: return "China PR";
	case eNationalAssociation::Colombia: return "Colombia";
	case eNationalAssociation::Comoros: return "Comoros";
	case eNationalAssociation::Congo: return "Congo";
	case eNationalAssociation::CongoDR: return "Congo DR";
	case eNationalAssociation::CookIslands: return "Cook Islands";
	case eNationalAssociation::CostaRica: return "Costa Rica";
	case eNationalAssociation::CoteDIvoire: return "Cote d'Ivoire";
	case eNationalAssociation::Croatia: return "Croatia";
	case eNationalAssociation::Cuba: return "Cuba";
	case eNationalAssociation::Curacao: return "Curacao";
	case eNationalAssociation::Cyprus: return "Cyprus";
	case eNationalAssociation::CzechRepublic: return "Czech Republic";
	case eNationalAssociation::Denmark: return "Denmark";
	case eNationalAssociation::Djibouti: return "Djibouti";
	case eNationalAssociation::Dominica: return "Dominica";
	case eNationalAssociation::DominicanRepublic: return "Dominican Republic";
	case eNationalAssociation::Ecuador: return "Ecuador";
	case eNationalAssociation::Egypt: return "Egypt";
	case eNationalAssociation::ElSalvador: return "El Salvador";
	case eNationalAssociation::England: return "England";
	case eNationalAssociation::EquatorialGuinea: return "Equatorial Guinea";
	case eNationalAssociation::Eritrea: return "Eritrea";
	case eNationalAssociation::Estonia: return "Estonia";
	case eNationalAssociation::Eswatini: return "Eswatini";
	case eNationalAssociation::Ethiopia: return "Ethiopia";
	case eNationalAssociation::FaroeIslands: return "Faroe Islands";
	case eNationalAssociation::Fiji: return "Fiji";
	case eNationalAssociation::Finland: return "Finland";
	case eNationalAssociation::France: return "France";
	case eNationalAssociation::Gabon: return "Gabon";
	case eNationalAssociation::Gambia: return "Gambia";
	case eNationalAssociation::Georgia: return "Georgia";
	case eNationalAssociation::Germany: return "Germany";
	case eNationalAssociation::Ghana: return "Ghana";
	case eNationalAssociation::Gibraltar: return "Gibraltar";
	case eNationalAssociation::Greece: return "Greece";
	case eNationalAssociation::Grenada: return "Grenada";
	case eNationalAssociation::Guam: return "Guam";
	case eNationalAssociation::Guatemala: return "Guatemala";
	case eNationalAssociation::Guinea: return "Guinea";
	case eNationalAssociation::GuineaBissau: return "Guinea-Bissau";
	case eNationalAssociation::Guyana: return "Guyana";
	case eNationalAssociation::Haiti: return "Haiti";
	case eNationalAssociation::Honduras: return "Honduras";
	case eNationalAssociation::HongKong: return "Hong Kong";
	case eNationalAssociation::Hungary: return "Hungary";
	case eNationalAssociation::Iceland: return "Iceland";
	case eNationalAssociation::India: return "India";
	case eNationalAssociation::Indonesia: return "Indonesia";
	case eNationalAssociation::Iran: return "Iran";
	case eNationalAssociation::Iraq: return "Iraq";
	case eNationalAssociation::Ireland: return "Ireland";
	case eNationalAssociation::Israel: return "Israel";
	case eNationalAssociation::Italy: return "Italy";
	case eNationalAssociation::Jamaica: return "Jamaica";
	case eNationalAssociation::Japan: return "Japan";
	case eNationalAssociation::Jordan: return "Jordan";
	case eNationalAssociation::Kazakhstan: return "Kazakhstan";
	case eNationalAssociation::Kenya: return "Kenya";
	case eNationalAssociation::Kiribati: return "Kiribati";
	case eNationalAssociation::Kosovo: return "Kosovo";
	case eNationalAssociation::Kuwait: return "Kuwait";
	case eNationalAssociation::Kyrgyzstan: return "Kyrgyzstan";
	case eNationalAssociation::Laos: return "Laos";
	case eNationalAssociation::Latvia: return "Latvia";
	case eNationalAssociation::Lebanon: return "Lebanon";
	case eNationalAssociation::Lesotho: return "Lesotho";
	case eNationalAssociation::Liberia: return "Liberia";
	case eNationalAssociation::Libya: return "Libya";
	case eNationalAssociation::Liechtenstein: return "Liechtenstein";
	case eNationalAssociation::Lithuania: return "Lithuania";
	case eNationalAssociation::Luxembourg: return "Luxembourg";
	case eNationalAssociation::Macau: return "Macau";
	case eNationalAssociation::Madagascar: return "Madagascar";
	case eNationalAssociation::Malawi: return "Malawi";
	case eNationalAssociation::Malaysia: return "Malaysia";
	case eNationalAssociation::Maldives: return "Maldives";
	case eNationalAssociation::Mali: return "Mali";
	case eNationalAssociation::Malta: return "Malta";
	case eNationalAssociation::MarshallIslands: return "Marshall Islands";
	case eNationalAssociation::Mauritania: return "Mauritania";
	case eNationalAssociation::Mauritius: return "Mauritius";
	case eNationalAssociation::Mexico: return "Mexico";
	case eNationalAssociation::Micronesia: return "Micronesia";
	case eNationalAssociation::Moldova: return "Moldova";
	case eNationalAssociation::Monaco: return "Monaco";
	case eNationalAssociation::Mongolia: return "Mongolia";
	case eNationalAssociation::Montenegro: return "Montenegro";
	case eNationalAssociation::Montserrat: return "Montserrat";
	case eNationalAssociation::Morocco: return "Morocco";
	case eNationalAssociation::Mozambique: return "Mozambique";
	case eNationalAssociation::Myanmar: return "Myanmar";
	case eNationalAssociation::Namibia: return "Namibia";
	case eNationalAssociation::Nauru: return "Nauru";
	case eNationalAssociation::Nepal: return "Nepal";
	case eNationalAssociation::Netherlands: return "Netherlands";
	case eNationalAssociation::NewCaledonia: return "New Caledonia";
	case eNationalAssociation::NewZealand: return "New Zealand";
	case eNationalAssociation::Nicaragua: return "Nicaragua";
	case eNationalAssociation::Niger: return "Niger";
	case eNationalAssociation::Nigeria: return "Nigeria";
	case eNationalAssociation::NorthKorea: return "North Korea";
	case eNationalAssociation::NorthMacedonia: return "North Macedonia";
	case eNationalAssociation::NorthernIreland: return "Northern Ireland";
	case eNationalAssociation::Norway: return "Norway";
	case eNationalAssociation::Oman: return "Oman";
	case eNationalAssociation::Pakistan: return "Pakistan";
	case eNationalAssociation::Palestine: return "Palestine";
	case eNationalAssociation::Panama: return "Panama";
	case eNationalAssociation::PapuaNewGuinea: return "Papua New Guinea";
	case eNationalAssociation::Paraguay: return "Paraguay";
	case eNationalAssociation::Peru: return "Peru";
	case eNationalAssociation::Philippines: return "Philippines";
	case eNationalAssociation::Poland: return "Poland";
	case eNationalAssociation::Portugal: return "Portugal";
	case eNationalAssociation::PuertoRico: return "Puerto Rico";
	case eNationalAssociation::Qatar: return "Qatar";
	case eNationalAssociation::Romania: return "Romania";
	case eNationalAssociation::Russia: return "Russia";
	case eNationalAssociation::Rwanda: return "Rwanda";
	case eNationalAssociation::SaintKittsAndNevis: return "Saint Kitts and Nevis";
	case eNationalAssociation::SaintLucia: return "Saint Lucia";
	case eNationalAssociation::SaintVincentAndTheGrenadines: return "Saint Vincent and the Grenadines";
	case eNationalAssociation::Samoa: return "Samoa";
	case eNationalAssociation::SanMarino: return "San Marino";
	case eNationalAssociation::SaoTomeAndPrincipe: return "Sao Tome and Principe";
	case eNationalAssociation::SaudiArabia: return "Saudi Arabia";
	case eNationalAssociation::Scotland: return "Scotland";
	case eNationalAssociation::Senegal: return "Senegal";
	case eNationalAssociation::Serbia: return "Serbia";
	case eNationalAssociation::Seychelles: return "Seychelles";
	case eNationalAssociation::SierraLeone: return "Sierra Leone";
	case eNationalAssociation::Singapore: return "Singapore";
	case eNationalAssociation::Slovakia: return "Slovakia";
	case eNationalAssociation::Slovenia: return "Slovenia";
	case eNationalAssociation::SolomonIslands: return "Solomon Islands";
	case eNationalAssociation::Somalia: return "Somalia";
	case eNationalAssociation::SouthAfrica: return "South Africa";
	case eNationalAssociation::SouthKorea: return "South Korea";
	case eNationalAssociation::SouthSudan: return "South Sudan";
	case eNationalAssociation::Spain: return "Spain";
	case eNationalAssociation::SriLanka: return "Sri Lanka";
	case eNationalAssociation::Sudan: return "Sudan";
	case eNationalAssociation::Suriname: return "Suriname";
	case eNationalAssociation::Sweden: return "Sweden";
	case eNationalAssociation::Switzerland: return "Switzerland";
	case eNationalAssociation::Syria: return "Syria";
	case eNationalAssociation::Taiwan: return "Taiwan";
	case eNationalAssociation::Tajikistan: return "Tajikistan";
	case eNationalAssociation::Tanzania: return "Tanzania";
	case eNationalAssociation::Thailand: return "Thailand";
	case eNationalAssociation::TimorLeste: return "Timor-Leste";
	case eNationalAssociation::Togo: return "Togo";
	case eNationalAssociation::Tonga: return "Tonga";
	case eNationalAssociation::TrinidadAndTobago: return "Trinidad and Tobago";
	case eNationalAssociation::Tunisia: return "Tunisia";
	case eNationalAssociation::Turkey: return "Turkey";
	case eNationalAssociation::Turkmenistan: return "Turkmenistan";
	case eNationalAssociation::TurksAndCaicosIslands: return "Turks and Caicos Islands";
	case eNationalAssociation::Tuvalu: return "Tuvalu";
	case eNationalAssociation::Uganda: return "Uganda";
	case eNationalAssociation::Ukraine: return "Ukraine";
	case eNationalAssociation::UnitedArabEmirates: return "United Arab Emirates";
	case eNationalAssociation::UnitedStates: return "United States";
	case eNationalAssociation::Uruguay: return "Uruguay";
	case eNationalAssociation::USVirginIslands: return "US Virgin Islands";
	case eNationalAssociation::Uzbekistan: return "Uzbekistan";
	case eNationalAssociation::Vanuatu: return "Vanuatu";
	case eNationalAssociation::VaticanCity: return "Vatican City";
	case eNationalAssociation::Venezuela: return "Venezuela";
	case eNationalAssociation::Vietnam: return "Vietnam";
	case eNationalAssociation::Wales: return "Wales";
	case eNationalAssociation::Yemen: return "Yemen";
	case eNationalAssociation::Zambia: return "Zambia";
	case eNationalAssociation::Zimbabwe: return "Zimbabwe";
#endif
	default: return "Unknown";
	}
}