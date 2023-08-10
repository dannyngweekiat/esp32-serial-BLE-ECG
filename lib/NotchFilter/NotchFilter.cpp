#include "NotchFilter.hpp"

// Filter generated using https://fiiir.com/
// Sampling frequency:  1000 Hz
// Cutoff frequency fL [Hz]:     45 Hz
// Transition bandwidth bL [Hz]: 10 Hz
// Cutoff frequency fH [Hz]:     55 Hz
// Transition bandwidth bH [Hz]: 10 Hz
// Window Type Hamming

#define NOTCH_FILTER_LENGTH 311

double h[] = {
    -0.000000051784663275,
    -0.000101561419912665,
    -0.000195758956557643,
    -0.000273457220196738,
    -0.000326714158520417,
    -0.000349589389363630,
    -0.000338761961302338,
    -0.000293950398695994,
    -0.000218086283395420,
    -0.000117208011663028,
    -0.000000061871579929,
    0.000122577580032057,
    0.000238828325937579,
    0.000336795572277498,
    0.000405748038295745,
    0.000437264873173377,
    0.000426233049175608,
    0.000371578085775923,
    0.000276627627298548,
    0.000149037643356651,
    0.000000252956115195,
    -0.000155476279482310,
    -0.000302447852473997,
    -0.000425107098879574,
    -0.000509730328881753,
    -0.000546006535484893,
    -0.000528329327493414,
    -0.000456631301956181,
    -0.000336635620505823,
    -0.000179460898134505,
    -0.000000589388922076,
    0.000181713588686408,
    0.000348368692273500,
    0.000481622746144188,
    0.000567109535383432,
    0.000595577164783135,
    0.000564056644113098,
    0.000476305613231717,
    0.000342445116473876,
    0.000177806583829819,
    0.000001108523220276,
    -0.000167825484317913,
    -0.000310526906284113,
    -0.000412204578389758,
    -0.000463618297105838,
    -0.000462239740768457,
    -0.000412521818927817,
    -0.000325211325626354,
    -0.000215768461802787,
    -0.000102083680941775,
    -0.000001789881512012,
    0.000070459766347882,
    0.000105353284646564,
    0.000100554697083699,
    0.000061328742481373,
    -0.000000000000000004,
    -0.000065750389266630,
    -0.000115577781781701,
    -0.000129826897178376,
    -0.000093092060038213,
    0.000002535502680540,
    0.000155053331467492,
    0.000351412610853447,
    0.000567965571532775,
    0.000772600788558497,
    0.000928441456449585,
    0.000998747936643665,
    0.000952460188050967,
    0.000769668437168898,
    0.000446236547364767,
    -0.000003162185005695,
    -0.000544198740317241,
    -0.001124601524115546,
    -0.001678495732765397,
    -0.002133095057801575,
    -0.002417134954689010,
    -0.002470147671103908,
    -0.002251479532267677,
    -0.001747873923256960,
    -0.000978509267787281,
    0.000003406221224148,
    0.001113036720979200,
    0.002240427372218538,
    0.003260732078558853,
    0.004047281795362566,
    0.004486189225327779,
    0.004490884237854533,
    0.004014843002276437,
    0.003060841814826821,
    0.001685340715559068,
    -0.000002935513702009,
    -0.001850542278371885,
    -0.003672615929407917,
    -0.005270859168813466,
    -0.006454513793135268,
    -0.007062151405432906,
    -0.006981860642016196,
    -0.006167421409695202,
    -0.004648348284844495,
    -0.002532240357476204,
    0.000001354640926595,
    0.002715444129173575,
    0.005337391856189908,
    0.007586057475928797,
    0.009201596883445178,
    0.009974788545176763,
    0.009772621909645937,
    0.008557115130048618,
    0.006394913484455370,
    0.003456109094154326,
    0.000001831784022923,
    -0.003638623888755252,
    -0.007099180709082711,
    -0.010013885554755478,
    -0.012055609151481671,
    -0.012972438318848353,
    -0.012617727239856741,
    -0.010970288178375372,
    -0.008142124175782433,
    -0.004372350814877626,
    -0.000007396136982093,
    0.004530944400370727,
    0.008786649637296761,
    0.012315762787649632,
    0.014733083114949239,
    0.015754305630563500,
    0.015228978710424956,
    0.013160484652623696,
    0.009710519427136853,
    0.005187129489955021,
    0.000017092211702241,
    -0.005294881690837378,
    -0.010216175003109108,
    -0.014240487689606934,
    -0.016940654643766897,
    -0.018014471625847697,
    -0.017318558917179028,
    -0.014886452599804093,
    -0.010928704758627207,
    -0.005814633275994475,
    -0.000037294104233067,
    0.005834952519888099,
    0.011215440956558687,
    0.015558945654077689,
    0.018417981307147269,
    0.019489744716096061,
    0.018648695327472046,
    0.015961222546105505,
    0.011680691020276732,
    0.006223197285713758,
    0.000126396239877369,
    -0.006004452257243442,
    -0.011557700515077582,
    -0.015977486105704652,
    -0.018820423158100077,
    0.980199077407586605,
    -0.018820423158100077,
    -0.015977486105704652,
    -0.011557700515077582,
    -0.006004452257243442,
    0.000126396239877369,
    0.006223197285713758,
    0.011680691020276739,
    0.015961222546105505,
    0.018648695327472042,
    0.019489744716096061,
    0.018417981307147269,
    0.015558945654077692,
    0.011215440956558687,
    0.005834952519888099,
    -0.000037294104233067,
    -0.005814633275994475,
    -0.010928704758627207,
    -0.014886452599804097,
    -0.017318558917179028,
    -0.018014471625847697,
    -0.016940654643766900,
    -0.014240487689606934,
    -0.010216175003109108,
    -0.005294881690837378,
    0.000017092211702239,
    0.005187129489955021,
    0.009710519427136853,
    0.013160484652623696,
    0.015228978710424957,
    0.015754305630563500,
    0.014733083114949244,
    0.012315762787649632,
    0.008786649637296761,
    0.004530944400370727,
    -0.000007396136982093,
    -0.004372350814877626,
    -0.008142124175782434,
    -0.010970288178375375,
    -0.012617727239856741,
    -0.012972438318848353,
    -0.012055609151481675,
    -0.010013885554755481,
    -0.007099180709082713,
    -0.003638623888755252,
    0.000001831784022923,
    0.003456109094154326,
    0.006394913484455370,
    0.008557115130048620,
    0.009772621909645937,
    0.009974788545176763,
    0.009201596883445179,
    0.007586057475928800,
    0.005337391856189909,
    0.002715444129173575,
    0.000001354640926595,
    -0.002532240357476204,
    -0.004648348284844495,
    -0.006167421409695204,
    -0.006981860642016198,
    -0.007062151405432907,
    -0.006454513793135270,
    -0.005270859168813467,
    -0.003672615929407917,
    -0.001850542278371885,
    -0.000002935513702009,
    0.001685340715559068,
    0.003060841814826821,
    0.004014843002276438,
    0.004490884237854535,
    0.004486189225327779,
    0.004047281795362566,
    0.003260732078558853,
    0.002240427372218538,
    0.001113036720979201,
    0.000003406221224148,
    -0.000978509267787281,
    -0.001747873923256960,
    -0.002251479532267676,
    -0.002470147671103910,
    -0.002417134954689011,
    -0.002133095057801575,
    -0.001678495732765398,
    -0.001124601524115547,
    -0.000544198740317241,
    -0.000003162185005695,
    0.000446236547364767,
    0.000769668437168898,
    0.000952460188050967,
    0.000998747936643665,
    0.000928441456449585,
    0.000772600788558497,
    0.000567965571532775,
    0.000351412610853447,
    0.000155053331467492,
    0.000002535502680540,
    -0.000093092060038213,
    -0.000129826897178376,
    -0.000115577781781701,
    -0.000065750389266630,
    -0.000000000000000004,
    0.000061328742481373,
    0.000100554697083699,
    0.000105353284646564,
    0.000070459766347882,
    -0.000001789881512012,
    -0.000102083680941776,
    -0.000215768461802787,
    -0.000325211325626354,
    -0.000412521818927817,
    -0.000462239740768457,
    -0.000463618297105839,
    -0.000412204578389758,
    -0.000310526906284114,
    -0.000167825484317913,
    0.000001108523220276,
    0.000177806583829819,
    0.000342445116473876,
    0.000476305613231716,
    0.000564056644113098,
    0.000595577164783135,
    0.000567109535383432,
    0.000481622746144188,
    0.000348368692273500,
    0.000181713588686408,
    -0.000000589388922076,
    -0.000179460898134505,
    -0.000336635620505824,
    -0.000456631301956181,
    -0.000528329327493414,
    -0.000546006535484892,
    -0.000509730328881754,
    -0.000425107098879574,
    -0.000302447852473997,
    -0.000155476279482311,
    0.000000252956115195,
    0.000149037643356651,
    0.000276627627298548,
    0.000371578085775924,
    0.000426233049175608,
    0.000437264873173377,
    0.000405748038295745,
    0.000336795572277498,
    0.000238828325937580,
    0.000122577580032058,
    -0.000000061871579929,
    -0.000117208011663028,
    -0.000218086283395420,
    -0.000293950398695994,
    -0.000338761961302338,
    -0.000349589389363630,
    -0.000326714158520417,
    -0.000273457220196738,
    -0.000195758956557643,
    -0.000101561419912665,
    -0.000000051784663275,
};

FIRCoefficients *createNotchFilterCoefficients()
{
    FIRCoefficients *coefficients = new FIRCoefficients(NOTCH_FILTER_LENGTH, h);
    return coefficients;
}
