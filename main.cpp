#include<iostream>
#include<graphics.h>
#include<vector>
#include<windows.h>
#pragma comment(lib,"winmm.lib")
#include <string>
#include <conio.h>

using namespace std;
 
//��ʼ����
constexpr auto swidth = 600;
constexpr auto sheight = 1100;

bool PointInRect(int x, int y, RECT& r)//�ж������ڵ����λ��
{
	return(r.left <= x && x <= r.right && r.top <= y && y <= r.bottom);
}

//��ײ��⣬����л��ص�
bool  RectDuangRect(RECT& r1, RECT& r2)
{
	RECT r;
	r.left = r1.left - (r2.right - r2.left);
	r.right = r1.right;
	r.top = r1.top - (r2.bottom - r2.top);
	r.bottom = r1.bottom;

	return (r.left < r2.left && r2.left <= r.right && r.top <= r2.top && r2.top <= r.bottom);
}
void Welcome()
{
	LPCTSTR title = _T("�ɻ���ս");
	LPCTSTR tplay = _T("��ʼ��Ϸ");
	LPCTSTR texit= _T("�˳���Ϸ");

	//���������
	//��������title
	RECT tplayr, texitr;
	void BeginBatchDraw();
	setbkcolor(WHITE);
	cleardevice();
	settextstyle(60, 0, _T("����"));
	settextcolor(BLACK);
	outtextxy(swidth / 2 - textwidth(title) / 2, sheight / 10, title);
	
	//��ʼ��Ϸ���˳���Ϸ
	settextstyle(40, 0, _T("����"));
	tplayr.left = swidth / 2 - textwidth(tplay)/2;
	tplayr.right = tplayr.left + textwidth(tplay);
	tplayr.top = sheight / 5*2;
	tplayr.bottom = tplayr.top + textheight(tplay);

	texitr.left = swidth / 2 - textwidth(texit) / 2;
	texitr.right = texitr.left + textwidth(texit);
	texitr.top = sheight / 5*3;
	texitr.bottom = texitr.top + textheight(texit);

	outtextxy(tplayr.left, tplayr.top,tplay);
	outtextxy(texitr.left, texitr.top, texit);

	EndBatchDraw();
	
	//�����ѭ��
	while (true)
	{
		ExMessage mess;
		getmessage(&mess, EM_MOUSE);
		if (mess.lbutton)
		{
			if (PointInRect(mess.x, mess.y, tplayr))
			{
				return;
			}
			if(PointInRect(mess.x,mess.y,texitr))
			{
				 exit(0);
			}
		}
	}
	//Sleep(100);
}

//��Ϸ��������
void Over(unsigned long long& kill)
{
	TCHAR* str = new TCHAR[128];
	_stprintf_s(str, 128, _T("��ɱ����%llu"), kill);
	settextcolor (RED);
	outtextxy(swidth / 2 - textwidth(str) / 2, sheight / 5, str);
	//�����¼���ȡ�����س����أ�
	LPCTSTR info = _T("���س�������");
		settextstyle(20, 0, _T("����"));
		outtextxy(swidth - textwidth(info), sheight - textheight(info), info);
		while (true)
		{
			ExMessage mess;
			getmessage(&mess, EM_KEY);
			if (mess.vkcode == 0x0D)
			{
				return;
			}

        }


}  

/*void Music()
{
	PlaySound(TEXT("B:\Game\Air\res\JustBlue.mp3"), NULL, SND_FILENAME | SND_ASYNC);
	system("pause");
	return;
}*/

//ʵ���ࣺ�������л���Ӣ�ۡ��ӵ�
class BK
{
public:
	BK(IMAGE &img)
		:img(img),y(-sheight)
	{

	}
		void Show()
		{
			if (y == 0) { y = -sheight; }
			y += 4;
			putimage(0, y, &img);
		}

private:
	IMAGE& img;
	int x=0, y;

};
class Hero
{
public:
	Hero(IMAGE& img)
		:img(img)
	{
		rect.left = swidth / 2 - img.getwidth() / 2;
		rect.top = sheight - img.getheight();
		rect.right = rect.left + img.getwidth();
		rect.bottom = sheight;
	}
	//������show
	void Show()
	{
		putimage(rect.left, rect.top, &img);

	}
	void Control()
	{
		ExMessage mess;
		if (peekmessage(&mess, EM_MOUSE))
		{
			rect.left = mess.x - img.getwidth() / 2;
			rect.top = mess.y - img.getheight() / 2;
			rect.right = rect.right = rect.left + img.getwidth();
			rect.bottom = rect.top + img.getheight();

		}

	}
	RECT& GetRect() 
	{
		return(rect);
	}
private:
	IMAGE& img;
	RECT rect;
};
class Enemy
{
public:
	Enemy(IMAGE& img,int x)
		:img(img)
	{
		rect.left = x;
		rect.right = rect.left + img.getwidth();
		rect.top = -img.getheight();
		rect.bottom = 0;
	}
	bool Show()
	{
		if (rect.top >= sheight)
		{
			return false;
		}
		rect.top += 4;  //�л��ƶ��ٶ�
		rect.bottom += 4;
		putimage(rect.left, rect.top, &img);
		return true;
	}
	RECT& GetRect()
	{
		return(rect);
	}

private:
	IMAGE& img;
	RECT rect;
};
class Bullet
{
public:
	Bullet(IMAGE&img,RECT pr)
		:img(img)
	{

		rect.left = pr.left + (pr.right - pr.left) / 2 - img.getwidth() / 2;
		rect.right = rect.left + img.getwidth();
		rect.top = pr.top - img.getheight();
		rect.bottom = rect.top + img.getheight();
	}
	bool Show()
	{
		if (rect.bottom <= 0)
		{
			return false;
		}
		rect.top -= 3;
		rect.bottom-=3;
		putimage(rect.left, rect.top, &img);
		return true;
	}
	RECT& GetRect()
	{
		return(rect);
	}


private:
	IMAGE& img;

	RECT rect;
};
bool addEnemy(vector<Enemy*>& es,IMAGE&enemyimg)
{
	Enemy* e = new Enemy(enemyimg, abs(rand()) % (swidth - enemyimg.getwidth()));
	
	for (auto& i : es)
	{
		if (RectDuangRect(i->GetRect(), e->GetRect()))
		{
			delete e;
			return false;
		}

	}
	es.push_back(e);
	return true;
}
bool Play()
{
	//Music();
	setbkcolor(WHITE);
	cleardevice();
	bool is_play = true;

	IMAGE heroimg, enemyimg, bkimg, bulletimg;
	loadimage(&heroimg, _T("B:\\Game\\Air\\res\\�ɻ�.jpg"));
	loadimage(&enemyimg, _T("B:\\Game\\Air\\res\\�л�.jpg"));
	loadimage(&bkimg, _T("B:\\Game\\Air\\res\\����2.jpg"));
	loadimage(&bulletimg, _T("B:\\Game\\Air\\res\\�ӵ�1.jpg"));

	//mciSendString(TEXT("play B:\Game\Air\res\JustBlue.mp3"), NULL, 0, NULL); 
	

	//ʵ�л�

	BK bk = BK(bkimg);
	Hero hp = Hero(heroimg);

	vector <Enemy*> es;
	vector<Bullet*> bs;
	int bsing = 0;

	unsigned long long kill=0;

	for (int i = 0; i < 5; i++)
	{
		addEnemy(es, enemyimg);
		//es.push_back(new Enemy(enemyimg, abs(rand()) % (swidth - enemyimg.getwidth())));
	}


	while (is_play)
	{

		bsing++;
		if (bsing == 20)
		{
			bsing = 0;
			bs.push_back(new Bullet(bulletimg, hp.GetRect()));
		}
		BeginBatchDraw();
		bk.Show();
		{
			ExMessage mess;
			if (peekmessage(&mess, EM_KEY))
			{
				if (mess.vkcode == 0x020)
				{
					while (true)
					{
						flushmessage();
						getmessage(&mess, EM_KEY);
						if (mess.vkcode == 0x20)
						{
							break;
						}
					}


				}
			}
		}

		Sleep(6);
		flushmessage(2);
		hp.Control();
		//Sleep(6);
		hp.Show();
		for (auto& i : bs)
		{
			i->Show();
		}
		auto it = es.begin();
		while (it != es.end())
		{
			if (RectDuangRect((*it)->GetRect(), hp.GetRect()))
			{
				is_play = false;
			}
			auto bit = bs.begin();
			while (bit != bs.end())
			{
				if (RectDuangRect((*bit)->GetRect(), (*it)->GetRect()))
				{
					delete(*it);
					es.erase(it);
					it = es.begin();
					delete(*bit);
					bs.erase(bit);

					kill++;

					break;
				}
				bit++;
			}
			if (!(*it)->Show())
			{
				delete(*it);
				it = es.erase(it);
			}
			it++;
		}
		for (int  i = 0; i <5-es.size() ; i++)
		{
			addEnemy(es, enemyimg);

			//es.push_back(new Enemy(enemyimg, abs(rand()) % (swidth - enemyimg.getwidth())));

		}
		EndBatchDraw();
	}
	Over(kill);
	return true;
}


int main()
	{

		initgraph(swidth, sheight, EX_NOMINIMIZE | EX_SHOWCONSOLE);//��ʼ������
		bool is_live = true;
		while (is_live)
		{
			Welcome();
			//play
			is_live = Play();
		
		}

		return 0;
	}