// CONFIX:EXENAME('TCAudioTest')

#include "TCAudioManager.h"
#include "TCOutput.h"
#include "TCMTLockable.h"
#include "TCSystem.h"

#include <algorithm>
#include <cmath>

#include "TCDebugNew.h"

class MTTraceTarget: public TC::Output::PrintTarget,
   public TC::MT::ObjectLevelLockable<MTTraceTarget>
{
public:
   MTTraceTarget()
   {
      m_stream = TC::Factory::CreateStdOutStream();
   }
   virtual void Print(const char* text)
   {
      Locker lock(this);
      m_stream << text << TC::endl;
   }
private:
   TC::StreamPtr m_stream;
};

bool RunAudioTest()
{
   TC::Audio::ManagerPtr manager = TC::Audio::Manager::Create();
   if (!manager)
   {
      return false;
   }
   
         TC::Audio::SoundPtr sound1 = manager->CreateSound("N:\\Musik\\Bravo Hits\\Bravo Hits 1\\01 _ Sandra -- Don't Be Aggressive.mp3");
   //TC::Audio::SoundPtr sound1 = manager->CreateSound("akgit.wav");
//    TC::Audio::SoundPtr sound11 = manager->CreateSound("akgit.wav");
//    TC::Audio::SoundPtr sound12 = manager->CreateSound("akgit.wav");
//    TC::Audio::SoundPtr sound13 = manager->CreateSound("akgit.wav");
//    TC::Audio::SoundPtr sound14 = manager->CreateSound("akgit.wav");
// 
//    TC::Audio::SoundPtr sound2 = manager->CreateSound("bass.wav");
//    TC::Audio::SoundPtr sound21 = manager->CreateSound("bass.wav");
//    TC::Audio::SoundPtr sound22 = manager->CreateSound("bass.wav");
//    TC::Audio::SoundPtr sound23 = manager->CreateSound("bass.wav");
//    TC::Audio::SoundPtr sound24 = manager->CreateSound("bass.wav");
// 
//    TC::Audio::SoundPtr sound3 = manager->CreateSound("loopst.wav");
//    TC::Audio::SoundPtr sound31 = manager->CreateSound("loopst.wav");
//    TC::Audio::SoundPtr sound32 = manager->CreateSound("loopst.wav");
//    TC::Audio::SoundPtr sound33 = manager->CreateSound("loopst.wav");
//    TC::Audio::SoundPtr sound34 = manager->CreateSound("loopst.wav");
// 
//    TC::Audio::SoundPtr sound4 = manager->CreateSound("pad.wav");
//    TC::Audio::SoundPtr sound41 = manager->CreateSound("pad.wav");
//    TC::Audio::SoundPtr sound42 = manager->CreateSound("pad.wav");
//    TC::Audio::SoundPtr sound43 = manager->CreateSound("pad.wav");
//    TC::Audio::SoundPtr sound44 = manager->CreateSound("pad.wav");
// 
//    TC::Audio::SoundPtr sound5 = manager->CreateSound("seq.wav");
//    TC::Audio::SoundPtr sound51 = manager->CreateSound("seq.wav");
//    TC::Audio::SoundPtr sound52 = manager->CreateSound("seq.wav");
//    TC::Audio::SoundPtr sound53 = manager->CreateSound("seq.wav");
//    TC::Audio::SoundPtr sound54 = manager->CreateSound("seq.wav");
// 
//    TC::Audio::SoundCompoundPtr sequence1 = manager->CreateSoundSequence("stest1");
//    sequence1->AddSound(sound1);
//    sequence1->AddSound(sound2);
//    sequence1->AddSound(sound5);
// 
//    TC::Audio::SoundCompoundPtr sequence2 = manager->CreateSoundSequence("stest2");
//    sequence2->AddSound(sound3);
//    sequence2->AddSound(sound4);
// 
//    TC::Audio::SoundCompoundPtr multitrack1 = manager->CreateSoundMultitrack("mtest1");
//    multitrack1->AddSound(sound1);
// 
//    multitrack1->AddSound(sound11);
//    multitrack1->AddSound(sound12);
//    multitrack1->AddSound(sound13);
//    multitrack1->AddSound(sound14);
// 
//    multitrack1->AddSound(sound2);
//    multitrack1->AddSound(sound21);
//    multitrack1->AddSound(sound22);
//    multitrack1->AddSound(sound23);
//    multitrack1->AddSound(sound24);
// 
//    multitrack1->AddSound(sound5);
//    multitrack1->AddSound(sound51);
//    multitrack1->AddSound(sound52);
//    multitrack1->AddSound(sound53);
//    multitrack1->AddSound(sound54);
// 
//    TC::Audio::SoundCompoundPtr multitrack2 = manager->CreateSoundMultitrack("mtest2");
//    multitrack2->AddSound(sound3);
//    multitrack2->AddSound(sound31);
//    multitrack2->AddSound(sound32);
//    multitrack2->AddSound(sound33);
//    multitrack2->AddSound(sound34);
// 
//    multitrack2->AddSound(sound4);
//    multitrack2->AddSound(sound41);
//    multitrack2->AddSound(sound42);
//    multitrack2->AddSound(sound43);
//    multitrack2->AddSound(sound44);

   {
             sound1->Play();
             sound1->SetLooping(true);
      //       sound2->Play();
      //       sound2->SetLooping(true);
      //       sound3->Play();
      //       sound3->SetLooping(true);
      //       sound4->Play();
      //       sound4->SetLooping(true);
      //       sound5->Play();
      //       sound5->SetLooping(true);
      // 
      //       sound1->Play();
      //       TC::System::Sleep(300);
      //       sound1->Pause();
      //       TC::System::Sleep(300);
      //       sound1->Play();
      //       sound3->Play();
      //       TC::System::Sleep(300);
      //       sound1->Stop();
      //       TC::System::Sleep(1000);
      // 
      //       sound1->Play();
      //       sound1->SetLooping(true);
   }

   while (sound1->IsPlaying())
   {
      TC::System::Sleep(100);
   }

//    manager->DeleteSound(sequence1);
//    manager->DeleteSound(sequence2);
// 
//    manager->DeleteSound(multitrack1);
//    manager->DeleteSound(multitrack2);
// 
    manager->DeleteSound(sound1);
//    manager->DeleteSound(sound11);
//    manager->DeleteSound(sound12);
//    manager->DeleteSound(sound13);
//    manager->DeleteSound(sound14);
// 
//    manager->DeleteSound(sound2);
//    manager->DeleteSound(sound21);
//    manager->DeleteSound(sound22);
//    manager->DeleteSound(sound23);
//    manager->DeleteSound(sound24);
// 
//    manager->DeleteSound(sound3);
//    manager->DeleteSound(sound31);
//    manager->DeleteSound(sound32);
//    manager->DeleteSound(sound33);
//    manager->DeleteSound(sound34);
// 
//    manager->DeleteSound(sound4);
//    manager->DeleteSound(sound41);
//    manager->DeleteSound(sound42);
//    manager->DeleteSound(sound43);
//    manager->DeleteSound(sound44);
// 
//    manager->DeleteSound(sound5);
//    manager->DeleteSound(sound51);
//    manager->DeleteSound(sound52);
//    manager->DeleteSound(sound53);
//    manager->DeleteSound(sound54);

   return true;
}

int main(int /*narg*/, char** /*argv*/)
{
   try
   {
      TC::Output::PrintTargetPtr trace_target(new MTTraceTarget);
      TC::Output::SetErrorTarget(trace_target);
      TC::Output::SetWarningTarget(trace_target);
      TC::Output::SetInfoTarget(trace_target);
      TC::Output::SetTraceTarget(trace_target);
      TC::Output::SetTraceLevel(50);

      RunAudioTest();
   }

   catch (std::exception& e)
   {
      TCERRORS("AudioTest", "Unhandled Exception: ");
      TCERRORS("AudioTest", "\t" << e.what());
   }

   TC::Output::SetErrorTarget(TC::Output::PrintTargetPtr());
   TC::Output::SetWarningTarget(TC::Output::PrintTargetPtr());
   TC::Output::SetInfoTarget(TC::Output::PrintTargetPtr());
   TC::Output::SetTraceTarget(TC::Output::PrintTargetPtr());

   return 0;
}

