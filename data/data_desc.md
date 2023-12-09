### Data descritption and notes

#### csd\_vuc\_zai\_2022 
RDPI = Yearly Average of Daily Intensities
* Value: AADT: 22106
* K-factor estimate: 12%
  * [Source](https://www.fhwa.dot.gov/policyinformation/pubs/pl18027_traffic_data_pocket_guide.pdf)
* Peak hour traffic (One direction): (0.12 * 22106) / 2 = 1326.36
* Design hourly volume
* 0.368 Vehicles / second
* Rounded to 1200 for exp. distribution with lamba = 1/3
* Road ID: 90118

Traffic comparison before/after D1 Turany - Dubna Skala:  
Avg Before: 21785.4  
Avg after: 14474  
Estimated relief: **33.56%**

#### Types of vehicles:
Acceleration of different vehicle categories:

|                           |    All    | Personal  |  Bus  |  Truck  |
|---------------------------|:---------:|:---------:|:-----:|:-------:|
| Count                     | 3 079 662 | 2 555 491 | 8 422 | 515 749 |
| Acceleration [Cell / s^2] |     X     |   0.14    | 0.05  |  0.04   |
| Deceleration [Cell / s^2] |     X     |   0.52    |  0.1  |  0.09   |
| Length       [Cell]       |     X     |     1     |   2   |    3    |

Sources:
* Vehicle distribution: **Holienčíková Katarína** [Štatistický úrad SR](https://www.statistics.sk)
* Acceleration / Deceleration [Acceleration-Deceleration Behaviour of Various Vehicle Types](https://www.sciencedirect.com/science/article/pii/S2352146517307937)

#### Other resources:
* [Cellular automata in traffic simulation](https://arxiv.org/pdf/1805.05555.pdf)  
Notes:
  * NaSch model
  * More complex models
* [NaSch model](https://www.researchgate.net/publication/263504490_A_cellular_automaton_model_for_freeway_traffic)
