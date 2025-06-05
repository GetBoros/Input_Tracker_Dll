//------------------------------------------------------------------------------------------------------------
module;

export module Tools;
//------------------------------------------------------------------------------------------------------------
export import <sstream>;
import <Windows.h>;
import Coroutine;
//------------------------------------------------------------------------------------------------------------
class AValueMonitor
{
public:
	void Check_Value_Async(const int* ValuePtr, int Target)
	{
		CurrentWaiter = AIntWaiter(ValuePtr, Target);
		CurrentWaiter.operator co_await();
	}

private:
	AIntWaiter CurrentWaiter;
};
//------------------------------------------------------------------------------------------------------------
export class AsTools
{
public:
	~AsTools();
	AsTools();

	void Throw();

	// TEMP || If last click don`t save cords, or if need save |
	void Read_Json();
	void Hook_Enable();
	void Hook_Disable();
	void Click_Point_Save();
	bool FFmpeg_Chank_List_Record(wchar_t **file_name_result);
	void FFmpeg_Chank_List_Stop();
	void Curl_Examples();  // !!! TASK Login and create coockies | Run tor bundler and receive our IP for feature examples

	void Clicker_Handler();

	static constexpr int Array_Length = 5;
	static int *Ptr_X_Cords;
	static int *Ptr_Y_Cords;
	static int *Array_X_Cords;
	static int *Array_Y_Cords;

	static HANDLE Fmpeg_Process;
	static HANDLE Fmpeg_Stdin;
	// TEMP END

	static wchar_t *Handle_Clipboard();  // !!! mem leak, need free memory or use smurf pointers

private:
	// !!! Create class what have only import filesystem to handle windows filesystem
	void Get_File_Unique_Name(const wchar_t *text_chank_lists, std::wstring &file_name);
	void Get_Ffmpeg_Cons_Command();
};
//------------------------------------------------------------------------------------------------------------

#pragma region TASKS
/*
	X - Five buttons:
		V - Save chunks
		X - Start recording desktop
		X - Make screen shoots
	X - Maybe move all file to folder if many re_jenna_00 - 99 it`s good
		- how to
	X - Make UI and start big refactoring
	X - Can add button to store .m3u8 url just create button with nick what store those url and when press start record?
	X - Please make every 10 second make screen shoot with ffmpeg thanks

*/
#pragma endregion

#pragma region TASKS_SAVE_DESTOP
/*
	X - Every 5 seconds save desktops
*/
#pragma endregion


#pragma region FFMPEG
//
/*
	ffplay -hwaccel qsv -i "https://edge6-hel.live.mmcdn.com/live-edge/amlst:sincere_shyness-sd-c1f6b213faeaf4d2aca39c22606c277265abb4fae1a8ac2e8c53deca2294dca5_trns_h264/chunklist_w1074660170_b5128000_t64RlBTOjMwLjA=.m3u8"
	
*/
#pragma endregion


#pragma region FFPROB_TEST_EXAMPLES
// HELP Used to get || scale | bitrate | codec | length | all in file | meta data ||
/*
	
	ffprob input.webp
	ffprobe -v quiet -print_format json -show_format -show_streams input.webp
	ffprobe -v error -select_streams v:0 -show_entries stream=width,height,bit_rate,codec_name -of default=noprint_wrappers=1 ""

*/
#pragma endregion

#pragma region FFPLAY_TEST_EXAMPLES
//
/*
	ffplay -user_agent "Boros" "https://edge6-hel.live.mmcdn.com/live-edge/amlst:sincere_shyness-sd-c1f6b213faeaf4d2aca39c22606c277265abb4fae1a8ac2e8c53deca2294dca5_trns_h264/chunklist_w1074660170_b5128000_t64RlBTOjMwLjA=.m3u8"

*/
#pragma endregion

#pragma region SACRIFICE_PACK_CLUB_START
/*
(() => {
  // --- Настройки ---
  const refreshDelayMs = 50;  // задержка между обновлениями, мс
  const postDonateDelayMs = 100; // доп. задержка после удачного доната, мс

  // --- Вспомогательные функции ---
  const log = msg => console.log(`[${new Date().toISOString()}] ${msg}`);
  const delay = ms => new Promise(res => setTimeout(res, ms));

  function findDonateButton() {
	 return Array.from(document.querySelectorAll('button'))
		.find(b => b.textContent.trim().includes('Пожертвовать карту'));
  }
  function findRefreshButton() {
	 return document.querySelector('.club__boost__refresh-btn');
  }

  // --- Контроль выполнения ---
  let stopped = false;
  window.stopDonations = () => {
	 stopped = true;
	 log('Скрипт остановлен пользователем');
  };

  // --- Основной асинхронный цикл ---
  async function donateLoop() {
	 log('Старт бесконечного цикла донатов');
	 while (!stopped) {
		const donateBtn = findDonateButton();
		if (donateBtn) {
		  donateBtn.click();
		  log('✔  Донат совершён');
		  // ждём, пока сайт обновит карту
		  await delay(postDonateDelayMs);
		  continue; // сразу пробуем донатить новую карту
		}

		const refreshBtn = findRefreshButton();
		if (refreshBtn) {
		  refreshBtn.click();
		  log('↻  Обновил задание');
		} else {
		  log('⚠️  Кнопка «Обновить» не найдена');
		}

		// ждём перед следующей попыткой
		await delay(refreshDelayMs);
	 }
  }

  // --- Запуск ---
  donateLoop();
  log(`Используй window.stopDonations() чтобы остановить цикл`);
})();

*/

// SACRIFICE PACK CLUB END
/**
window.stopDonations();

 */

#pragma endregion

#pragma region MyRegion
//
/*
(function() {
  // Интервал ожидания после нажатия "Обновить" (в миллисекундах)
  const CHECK_DELAY = 250;

  // Селекторы для кнопок
  const REFRESH_SELECTOR = '.club__boost__refresh-btn'; // кнопка "Обновить задание"
  const DONATE_SELECTOR  = '.club__boost-btn';          // кнопка "Пожертвовать карту"

  // Счётчик успешных случаев, когда кнопка "Пожертвовать карту" стала доступна
  let donateCount = 0;

  // Основная функция одного цикла
  function doCycle() {
	 // Чистим консоль
	 console.clear();

	 // Выводим в самом верху число уже обнаруженных возможностей пожертвовать
	 console.log(`Пожертвуванно карт: ${donateCount}`);
	 console.log('Обновляем');

	 // Ищем и кликаем кнопку "Обновить задание"
	 const refreshBtn = document.querySelector(REFRESH_SELECTOR);
	 if (refreshBtn) {
		try {
		  refreshBtn.click();
		} catch (e) {
		  console.error('[Ошибка при клике на "Обновить задание"]', e);
		  // Если при попытке клика возникла ошибка, всё равно попытаемся запустить следующий цикл
		}
	 } else {
		console.log('Кнопка "Обновить задание" не найдена. Остановлено.');
		return;
	 }

	 // Ждём CHECK_DELAY мс, чтобы DOM успела обновиться и (возможно) появилась кнопка "Пожертвовать карту"
	 setTimeout(() => {
		const donateBtn = document.querySelector(DONATE_SELECTOR);

		if (donateBtn) {
		  // Кнопка "Пожертвовать карту" найдена — кликаем и увеличиваем счётчик
		  try {
			 donateBtn.click();
			 donateCount++;
			 console.log('🔹 Можно пожертвовать! Кликаем...');
		  } catch (e) {
			 console.error('[Ошибка при клике на "Пожертвовать карту"]', e);
		  }
		} else {
		  // Кнопки нет — логируем и ждём
		  console.log(`Карты нету → ждем ${CHECK_DELAY} мс`);
		}

		// После задержки снова запускаем новый цикл
		setTimeout(doCycle, CHECK_DELAY);
	 }, CHECK_DELAY);
  }

  // Запускаем первый цикл
  console.log('Скрипт запущен. Для остановки — перезагрузите страницу.');
  doCycle();
})();

*/
#pragma endregion

