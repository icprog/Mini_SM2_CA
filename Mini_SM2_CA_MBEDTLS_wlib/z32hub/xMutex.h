// ��������
// �ؼ����ݣ�����ı���
class xMutex
{
private:
	HANDLE SynMutex;

public:
	xMutex();
	~xMutex();
	void Build(LPCTSTR mutexName); // ����������
	bool EnterSynCode();	// ����ͬ�������������޵ȴ�
	bool EnterSynCode(DWORD WaitTime);	// ����ͬ�������������޵ȴ�
	bool LeaveSynCode();	// �뿪�ؼ�������
};
