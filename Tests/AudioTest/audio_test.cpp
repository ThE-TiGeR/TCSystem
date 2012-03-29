#include "TCAudioManager.h"
#include "TCOutput.h"
#include "TCMTLockable.h"
#include "TCSystem.h"

#include <algorithm>
#include <cmath>

#include "TCDebugNew.h"

class MTTraceTarget: public tc::output::PrintTarget,
   public tc::multi_threading::ObjectLevelLockable<MTTraceTarget>
{
public:
   MTTraceTarget()
   {
      m_stream = tc::factory::CreateStdOutStream();
   }
   virtual void Print(const char* text)
   {
      Locker lock(this);
      m_stream << text << tc::endl;
   }
private:
   tc::StreamPtr m_stream;
};

bool RunAudioTest()
{
   tc::audio::ManagerPtr manager = tc::audio::Manager::Create();
   if (!manager)
   {
      return false;
   }
   
         tc::audio::SoundPtr sound1 = manager->CreateSound("D:\\Thomas\\Musik\\Anastacia\\Anastacia (2004) - Pop\\03 - Anastacia - Time - www.torrentazos.com.mp3");
   //tc::audio::SoundPtr sound1 = manager->CreateSound("akgit.wav");
//    tc::audio::SoundPtr sound11 = manager->CreateSound("akgit.wav");
//    tc::audio::SoundPtr sound12 = manager->CreateSound("akgit.wav");
//    tc::audio::SoundPtr sound13 = manager->CreateSound("akgit.wav");
//    tc::audio::SoundPtr sound14 = manager->CreateSound("akgit.wav");
// 
//    tc::audio::SoundPtr sound2 = manager->CreateSound("bass.wav");
//    tc::audio::SoundPtr sound21 = manager->CreateSound("bass.wav");
//    tc::audio::SoundPtr sound22 = manager->CreateSound("bass.wav");
//    tc::audio::SoundPtr sound23 = manager->CreateSound("bass.wav");
//    tc::audio::SoundPtr sound24 = manager->CreateSound("bass.wav");
// 
//    tc::audio::SoundPtr sound3 = manager->CreateSound("loopst.wav");
//    tc::audio::SoundPtr sound31 = manager->CreateSound("loopst.wav");
//    tc::audio::SoundPtr sound32 = manager->CreateSound("loopst.wav");
//    tc::audio::SoundPtr sound33 = manager->CreateSound("loopst.wav");
//    tc::audio::SoundPtr sound34 = manager->CreateSound("loopst.wav");
// 
//    tc::audio::SoundPtr sound4 = manager->CreateSound("pad.wav");
//    tc::audio::SoundPtr sound41 = manager->CreateSound("pad.wav");
//    tc::audio::SoundPtr sound42 = manager->CreateSound("pad.wav");
//    tc::audio::SoundPtr sound43 = manager->CreateSound("pad.wav");
//    tc::audio::SoundPtr sound44 = manager->CreateSound("pad.wav");
// 
//    tc::audio::SoundPtr sound5 = manager->CreateSound("seq.wav");
//    tc::audio::SoundPtr sound51 = manager->CreateSound("seq.wav");
//    tc::audio::SoundPtr sound52 = manager->CreateSound("seq.wav");
//    tc::audio::SoundPtr sound53 = manager->CreateSound("seq.wav");
//    tc::audio::SoundPtr sound54 = manager->CreateSound("seq.wav");
// 
//    tc::audio::SoundCompoundPtr sequence1 = manager->CreateSoundSequence("stest1");
//    sequence1->AddSound(sound1);
//    sequence1->AddSound(sound2);
//    sequence1->AddSound(sound5);
// 
//    tc::audio::SoundCompoundPtr sequence2 = manager->CreateSoundSequence("stest2");
//    sequence2->AddSound(sound3);
//    sequence2->AddSound(sound4);
// 
//    tc::audio::SoundCompoundPtr multitrack1 = manager->CreateSoundMultitrack("mtest1");
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
//    tc::audio::SoundCompoundPtr multitrack2 = manager->CreateSoundMultitrack("mtest2");
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
      //       tc::system::Sleep(300);
      //       sound1->Pause();
      //       tc::system::Sleep(300);
      //       sound1->Play();
      //       sound3->Play();
      //       tc::system::Sleep(300);
      //       sound1->Stop();
      //       tc::system::Sleep(1000);
      // 
      //       sound1->Play();
      //       sound1->SetLooping(true);
   }

   while (sound1->IsPlaying())
   {
      tc::system::Sleep(100);
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
      tc::output::PrintTargetPtr trace_target(new MTTraceTarget);
      tc::output::SetErrorTarget(trace_target);
      tc::output::SetWarningTarget(trace_target);
      tc::output::SetInfoTarget(trace_target);
      tc::output::SetTraceTarget(trace_target);
      tc::output::SetTraceLevel(50);

      RunAudioTest();
   }

   catch (std::exception& e)
   {
      TCERRORS("AudioTest", "Unhandled Exception: ");
      TCERRORS("AudioTest", "\t" << e.what());
   }

   tc::output::SetErrorTarget(tc::output::PrintTargetPtr());
   tc::output::SetWarningTarget(tc::output::PrintTargetPtr());
   tc::output::SetInfoTarget(tc::output::PrintTargetPtr());
   tc::output::SetTraceTarget(tc::output::PrintTargetPtr());

   return 0;
}

