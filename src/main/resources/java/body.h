package __PACKAGE__;

import java.util.Arrays; // For debugging only
import java.nio.ByteBuffer;
import __RUNTIME__.Utils;
import __RUNTIME__.RagelParser;

@SuppressWarnings("unused")
public abstract class __CLASS_NAME__ extends RagelParser {
	private int cs;

MARKS(USE_MARK,)

	protected final void init() {
		%% write init;
	}

  @Override
  public boolean execute(ByteBuffer buffer) {
    return resume(buffer);
  }

	public final boolean resume(ByteBuffer data) {
		int p = data.position(), pe = data.limit();
		int eof = -1;
		boolean fired = false;

		%% write exec;
		
		data.position(p);
		return fired;
	}

	%% write data;

	public static __CLASS_NAME__ getTestClass(final boolean binaryMode) {
		return new __CLASS_NAME__() {
			void write(String prefix, byte[] data) {
				System.out.print(prefix + ": ");
				System.out.println( binaryMode ? Arrays.toString(data) : "'" + new String(data) + "'");
			}

			void write(String prefix, int value) {
				System.out.print(prefix + ": ");
				System.out.println( value );
			}
			
			@Override
			protected void on_new_request() {
				System.out.print("NEW REQUEST: ");
				System.out.println( S(__MACHINE_NAME__) );
			}

			@Override
			protected void on_request_completed() {
				System.out.print("END REQUEST: ");
				System.out.println( S(__MACHINE_NAME__) );
			}

			MARKS(TEST_IMPL, )
		};
	}
}
