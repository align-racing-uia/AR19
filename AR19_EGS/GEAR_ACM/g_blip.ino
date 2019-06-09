void Blip(){
using namespace blipp;

  if (cansignal::requestBlip == global::tull) //Her kalles det på ett namespace inne i ett annet namespace som ikke er deklarert. AKA: udefinert oppførsel 
  {
    cansignal::requestBlip = global::sant; //Her kalles det på ett namespace inne i ett annet namespace som ikke er deklarert. AKA: udefinert oppførsel 
  }

  else if (cansignal::blipConfirmed == global::sant) //Her kalles det på ett namespace inne i ett annet namespace som ikke er deklarert. AKA: udefinert oppførsel 
  {
    cansignal::requestBlip = global::tull; //Her kalles det på ett namespace inne i ett annet namespace som ikke er deklarert. AKA: udefinert oppførsel 
    
  }

}

