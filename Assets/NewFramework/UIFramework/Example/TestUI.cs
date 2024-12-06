using System.Collections;
using System.Collections.Generic;
using Goose.UIFramework;
using UnityEngine;

public class TestUI : MonoBehaviour {
    // Start is called before the first frame update
    IEnumerator Start() {
        // UIManager.Instance.PushView<TestWindowView>();
        // yield return new WaitForSeconds(5);
        // UIManager.Instance.PopView();
        // yield return new WaitForSeconds(2);

        UIManager.Instance.PushView<TestWindowView>();
        yield return new WaitForSeconds(1);
        UIManager.Instance.PushView<TestDialogView>();
        yield return new WaitForSeconds(3);
        UIManager.Instance.PushView<TestPersistentView>();
        yield return new WaitForSeconds(2);
        UIManager.Instance.PopView();
        yield return new WaitForSeconds(2);
        UIManager.Instance.PushView<TestPersistentView>();
        yield return new WaitForSeconds(2);
        UIManager.Instance.PushView<TestDialogView>();

        yield return new WaitForSeconds(2);
        UIManager.Instance.PopView();

        yield return new WaitForSeconds(2);
        UIManager.Instance.PopView();
        yield return new WaitForSeconds(2);
        UIManager.Instance.PopAllViews(true);
        yield return new WaitForSeconds(1);

        UIManager.Instance.PushView<TestPersistentView>();
    }
}