void Blip(){
using namespace blipp;

if (cansignal::requestBlip == global::tull)
{
  cansignal::requestBlip = global::sant;
}

else if (cansignal::blipConfirmed == global::sant)
{
  cansignal::requestBlip = global::tull;
  
}



}

